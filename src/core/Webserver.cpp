#include "Webserver.hpp"

Webserver::Webserver(const Webserver &rhs) : configs_(rhs.configs_) {
  *this = rhs;
}

Webserver::~Webserver() {}

Webserver::Webserver(ConfigVector &configs)
    : socketNum_(0),
      serverSocketNum_(0),
      clientSocketNum_(0),
      socketOpt_(1),
      configs_(configs) {
  for (std::size_t i = 0; i < MAX_CLIENTS; ++i) {
    fds_[i].fd = -1;
    fds_[i].events = POLLIN;
    fds_[i].revents = 0;
  }
  for (ConfigVector::iterator it = configs.begin(); it != configs.end(); ++it) {
    if (socketNum_ < MAX_CLIENTS) {
      createServerSocket_(Sockets_[socketNum_], it->getPort(),
                          it->getTimeout());

    } else {
      error_("Too many Server Sockets, no client connection will be possible");
    }
  }
  startServerRoutine_();
}

Webserver &Webserver::operator=(const Webserver &rhs) {
  this->socketNum_ = rhs.socketNum_;
  this->serverSocketNum_ = rhs.serverSocketNum_;
  this->clientSocketNum_ = rhs.clientSocketNum_;
  this->socketOpt_ = rhs.socketOpt_;
  this->configs_ = rhs.configs_;
  return *this;
}

void Webserver::createServerSocket_(Socket &serverSocket, int port,
                                    double timeout) {
  memset(&serverSocket.socketaddr_, 0, sizeof(serverSocket.socketaddr_));
  serverSocket.socketaddr_.sin_family = AF_INET;
  serverSocket.socketaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSocket.socketaddr_.sin_port = htons(port);
  serverSocket.listeningSocket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket.listeningSocket_ == -1) {
    error_("Error: Socket creation failed");
  }

#ifdef __APPLE__
  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_NOSIGPIPE,
                 &serverSocket.socketOpt_,
                 sizeof(serverSocket.socketOpt_)) == -1) {
    error_("Error: Setting SO_NOSIGPIPE");
  }
#endif

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEADDR,
                 &serverSocket.socketOpt_,
                 sizeof(serverSocket.socketOpt_)) == -1) {
    error_("Error: Setting SO_REUSEADDR");
  }

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEPORT,
                 &serverSocket.socketOpt_,
                 sizeof(serverSocket.socketOpt_)) == -1) {
    error_("Error: Setting SO_REUSEPORT");
  }

  if (bind(serverSocket.listeningSocket_,
           (struct sockaddr *)&serverSocket.socketaddr_,
           sizeof(serverSocket.socketaddr_)) != 0) {
    error_("Error: Could not bind socket");
  }

  if (listen(serverSocket.listeningSocket_, 25) != 0) {
    error_("Error: Listen");
  }

  if (fcntl(serverSocket.listeningSocket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) ==
      -1) {
    error_("Error: Setting socket to nonblocking");
  }
  this->fds_[socketNum_].fd = serverSocket.listeningSocket_;
  serverSocket.fd_ = serverSocket.listeningSocket_;
  std::string servername = this->configs_[this->socketNum_].getServerName();
  serverSocket.setServerAddress(servername);
  serverSocket.socketIndex_ = socketNum_;
  serverSocket.socketType_ = SERVER;
  serverSocket.configId_ = this->socketNum_;
  serverSocket.timeout_ = timeout;
  this->socketNum_++;
  this->serverSocketNum_++;
}

void Webserver::createClientSocket_(Socket &serverSocket) {
  if (socketNum_ >= MAX_CLIENTS) return;
  socklen_t boundServerAddress_len = sizeof(serverSocket.socketaddr_);
  int new_client_sock;
  if ((new_client_sock = accept(serverSocket.fd_,
                                (struct sockaddr *)&serverSocket.socketaddr_,
                                &boundServerAddress_len)) == -1)
    error_("Accept Error");
  printVerbose("opened new Socket ", new_client_sock);
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    error_("Error: Setting socket to nonblocking");
  }
  this->Sockets_[socketNum_].fd_ = new_client_sock;
  this->fds_[socketNum_].fd = new_client_sock;
  this->Sockets_[socketNum_].socketIndex_ = socketNum_;
  this->Sockets_[socketNum_].socketType_ = CLIENT;
  this->Sockets_[socketNum_].setTimestamp();
  this->Sockets_[socketNum_].timeout_ = serverSocket.timeout_;
  socketNum_++;
  clientSocketNum_++;
  if (VERBOSE) {
    std::cout << "Connection Established with "
              << this->Sockets_[socketNum_ - 1].fd_ << std::endl;
  }
}

void Webserver::startServerRoutine_() {
  while (serverRunning) {
    int ret = poll(this->fds_, this->socketNum_, 10000);
    if (ret == -1) {
      if (serverRunning) printVerbose("poll error", "");
      break;
    }
    checkPending_();
    for (size_t i = 0; i < socketNum_; ++i) {
      if (this->fds_[i].revents == POLLIN) {
        if (Sockets_[i].socketType_ == SERVER) {
          createClientSocket_(Sockets_[i]);
        } else {
          existingConnection_(Sockets_[i], fds_[i], i);
        }
      }
      checkTimeoutClients();
    }
  }
}

std::string Webserver::createResponse_(Socket &socket) {
  Request request(socket.reqStatus.buffer);
  if (request.isKeepAlive()) socket.keepAlive_ = true;
  int conf = this->getConfigId_(request);
  Location loc = this->configs_[conf].getLocationByPath(request.getPath());
  std::cout << loc << std::endl;
  Response resobj(request, loc);
  std::string response = resobj.getHeader() + resobj.getBody();
  return response;
}

void Webserver::sendResponse_(Socket &socket, pollfd &pollfd, size_t &i) {
  socket.dataSend_ =
      send(socket.fd_, socket.response_.c_str(), socket.response_.size(), 0);

  if (socket.dataSend_ == static_cast<std::size_t>(-1)) {
    std::cout << "send failed" << std::endl;
    closeConnection_(socket, pollfd, i);
  } else if (socket.dataSend_ < socket.response_.size()) {
    socket.pendingSend_ = true;
    socket.response_ = socket.response_.substr(socket.dataSend_);
    socket.setTimestamp();
    pollfd.events = POLLOUT;
  } else {
    socket.pendingSend_ = false;
    socket.setTimestamp();
    pollfd.events = POLLIN;
    if (!socket.getKeepAlive()) closeConnection_(socket, pollfd, i);
  }
}

bool Webserver::existingConnection_(Socket &socket, pollfd &pollfd, size_t &i) {
  size_t currentBytes;
  if (receiveRequest(socket, currentBytes) || socket.pendingSend_) {
    printVerbose("connection on socket ", socket.fd_);
    socket.setTimestamp();
    if (!socket.pendingSend_) socket.response_ = this->createResponse_(socket);
    this->sendResponse_(socket, pollfd, i);
  }
  if (currentBytes == static_cast<std::size_t>(-1) &&
      (errno == EWOULDBLOCK || errno == EAGAIN) && VERBOSE) {
    std::cout << "BLOCKER: " << socket.fd_ << std::endl;
  } else if (currentBytes == 0 && !socket.reqStatus.pendingReceive) {
    if (!socket.getKeepAlive()) closeConnection_(socket, pollfd, i);
    return false;
  }
  return true;
}

void Webserver::closeConnection_(Socket &socket, pollfd &pollfd, size_t &i) {
  printVerbose("Connection closing on Socket ", socket.fd_);
  close(pollfd.fd);
  socket.socketType_ = UNUSED;
  for (size_t j = i; j < MAX_CLIENTS - 1; ++j) {
    this->fds_[j] = this->fds_[j + 1];
    this->Sockets_[j] = this->Sockets_[j + 1];
  }
  this->fds_[MAX_CLIENTS - 1].fd = -1;
  this->Sockets_[MAX_CLIENTS - 1].setIdle();
  socketNum_--;
  clientSocketNum_--;
}

void Webserver::checkPending_() {
  for (size_t i = 0; i < socketNum_; ++i) {
    if (this->Sockets_[i].pendingSend_) {
      this->sendResponse_(this->Sockets_[i], fds_[i], i);
      return;
    }
  }
}

void Webserver::checkTimeoutClients() {
  for (size_t i = 0; i < socketNum_; ++i) {
    if (this->Sockets_[i].socketType_ == CLIENT &&
        this->Sockets_[i].checkTimeout()) {
      printVerbose("Timeout of Client Socket: ", this->Sockets_[i].fd_);
      closeConnection_(this->Sockets_[i], this->fds_[i], i);
      break;
    }
  }
}

int Webserver::getConfigId_(const Request &request) {
  std::string toFind;
  try {
    toFind = request["Host"];
  } catch (std::exception &) {
    return (0);
  }
  for (int i = 0; i < MAX_CLIENTS; ++i) {
    if (toFind == this->Sockets_[i].serverAddress_)
      return (this->Sockets_[i].configId_);
  }
  return (0);
}

void Webserver::error_(const std::string &error) {
  printVerbose(error, "");
  exit(EXIT_FAILURE);
}
