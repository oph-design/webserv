#include "Webserver.hpp"

Webserver::Webserver(const Webserver &rhs) : configs_(rhs.configs_) {
  *this = rhs;
}

Webserver::~Webserver() {}

Webserver::Webserver(ConfigVector &configs)
    : serverSocketNum_(0),
      clientSocketNum_(0),
      socketOpt_(1),
      configs_(configs) {
  for (size_t i = 0; i < MAX_CLIENTS; ++i) {
    fds_[i].fd = -1;
    fds_[i].events = POLLIN;
    fds_[i].revents = 0;
  }

  for (ConfigVector::iterator it = configs.begin(); it != configs.end(); ++it) {
    size_t index = getFreeSocket();
    if (index != MAX_CLIENTS) {
      createServerSocket_(Sockets_[index], it->getPort(), it->getTimeout());
    } else {
      error_("Too many Server Sockets, no client connection will be possible");
    }
  }
  startServerRoutine_();
}

Webserver &Webserver::operator=(const Webserver &rhs) {
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

  if (listen(serverSocket.listeningSocket_, 200) != 0) {
    error_("Error: Listen");
  }

  if (fcntl(serverSocket.listeningSocket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) ==
      -1) {
    error_("Error: Setting socket to nonblocking");
  }
  size_t index = this->serverSocketNum_;
  this->fds_[index].fd = serverSocket.listeningSocket_;
  this->fds_[index].events = POLLIN;
  this->fds_[index].revents = 0;
  serverSocket.fd_ = serverSocket.listeningSocket_;
  std::string servername = this->configs_[index].getServerName();
  serverSocket.setServerAddress(servername);
  serverSocket.socketIndex_ = index;
  serverSocket.socketType_ = SERVER;
  serverSocket.configId_ = index;
  serverSocket.timeout_ = timeout;
  this->serverSocketNum_++;
}

void Webserver::createClientSocket_(Socket &serverSocket) {
  if (getFreeSocket() == MAX_CLIENTS) return;
  socklen_t boundServerAdress_len = sizeof(serverSocket.socketaddr_);
  int new_client_sock;
  if ((new_client_sock = accept(serverSocket.fd_,
                                (struct sockaddr *)&serverSocket.socketaddr_,
                                &boundServerAdress_len)) == -1)
    error_("Accept Error");
  printVerbose("opened new Socket ", new_client_sock);
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    error_("Error: Setting socket to nonblocking");
  }
  size_t index = getFreeSocket();
  this->Sockets_[index].fd_ = new_client_sock;
  this->fds_[index].fd = new_client_sock;
  this->Sockets_[index].socketIndex_ = index;
  this->Sockets_[index].socketType_ = CLIENT;
  this->Sockets_[index].setTimestamp();
  this->Sockets_[index].timeout_ = serverSocket.timeout_;
  clientSocketNum_++;
  if (VERBOSE) {
    std::cout << "Connection Established with " << this->Sockets_[index].fd_
              << std::endl;
  }
}

size_t Webserver::getFreeSocket() {
  for (size_t i = 0; i < MAX_CLIENTS; ++i) {
    if (this->fds_[i].fd == -1) {
      return i;
    }
  }
  return MAX_CLIENTS;
}

void Webserver::startServerRoutine_() {
  while (serverRunning) {
    int ret = poll(this->fds_, MAX_CLIENTS, 10000);
    if (ret == -1) {
      if (serverRunning) printVerbose("poll error", "");
      break;
    }
    for (size_t i = 0; i < MAX_CLIENTS; ++i) {
      if (pollError_(i))
        ;
      else if (this->fds_[i].revents == POLLIN) {
        if (Sockets_[i].socketType_ == SERVER) {
          createClientSocket_(Sockets_[i]);
        } else {
          receiveRequest_(Sockets_[i], fds_[i], i);
        }
      } else if (this->fds_[i].revents == POLLOUT)
        handlePollout(this->Sockets_[i], this->fds_[i], i);
    }
    checkTimeoutClients();
  }
}

bool Webserver::pollError_(size_t &i) {
  int res = 0;
  if (this->fds_[i].revents & POLLERR) {
    std::cout << "Error: POLLERR" << std::endl;
    res = 1;
  } else if (this->fds_[i].revents & POLLPRI) {
    std::cout << "Error: POLLPRI" << std::endl;
    res = 1;
  } else if (this->fds_[i].revents & POLLNVAL) {
    std::cout << "Error: POLLNVAL" << std::endl;
    res = 1;
  } else if (this->fds_[i].revents & POLLHUP) {
    std::cout << "Error: POLLHUP" << std::endl;
    res = 1;
  }
  if (res != 0) {
    closeConnection_(Sockets_[i], fds_[i], i);
    return true;
  }
  return false;
}

std::string Webserver::createResponse_(Socket &socket) {
  Request request(socket.reqStatus.buffer);
  if (request.isKeepAlive()) socket.keepAlive_ = true;
  int conf = this->getConfigId_(request);
  Location loc = this->configs_[conf].getLocationByPath(request.getPath());
  Response resobj(request, loc);
  std::string response = resobj.getHeader() + resobj.getBody();
  return response;
}

void Webserver::handlePollout(Socket &socket, pollfd &pollfd, size_t &i) {
	if (socket.pendingSend_ == true) {
    this->sendResponse_(socket, pollfd, i);
    socket.setTimestamp();
  }
  else if (socket.pendingSend_ == false) {
    socket.response_ = this->createResponse_(socket);
    this->sendResponse_(socket, pollfd, i);
    socket.setTimestamp();
  }
}

void Webserver::sendResponse_(Socket &socket, pollfd &pollfd, size_t &i) {
	socket.dataSend_ =
      send(socket.fd_, socket.response_.c_str(), socket.response_.size(), 0);
  if (socket.dataSend_ == static_cast<std::size_t>(-1)) {
		printVerbose("send failed", socket.fd_);
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
    if (socket.getKeepAlive() == false) closeConnection_(socket, pollfd, i);
  }
}

bool Webserver::receiveRequest_(Socket &socket, pollfd &pollfd, size_t &i) {
  size_t currentBytes;
  if (receiveRequest(socket, currentBytes)) {
    printVerbose("connection on socket ", socket.fd_);
    socket.setTimestamp();
    pollfd.events = POLLOUT;
  }
  if (currentBytes == static_cast<std::size_t>(-1) &&
      (errno == EWOULDBLOCK || errno == EAGAIN) && VERBOSE) {
    std::cout << "BLOCKER: " << socket.fd_ << std::endl;
  } else if (currentBytes == 0 && socket.reqStatus.pendingReceive == false) {
    if (socket.getKeepAlive() == false) closeConnection_(socket, pollfd, i);
    return false;
  }
  return true;
}

void Webserver::closeConnection_(Socket &socket, pollfd &pollfd, size_t &i) {
  (void)i;
  printVerbose("Connection closing on Socket ", socket.fd_);
  shutdown(pollfd.fd, SHUT_RDWR);
  close(pollfd.fd);
  pollfd.fd = -1;
  pollfd.events = POLLIN;
  socket.setIdle();
  clientSocketNum_--;
}

bool Webserver::checkPending_() {
  for (size_t i = 0; i < MAX_CLIENTS; ++i) {
    if (this->Sockets_[i].pendingSend_ == true &&
        this->Sockets_[i].socketType_ != UNUSED) {
      this->sendResponse_(this->Sockets_[i], fds_[i], i);
      return true;
    }
  }
  return false;
}

void Webserver::checkTimeoutClients() {
  for (size_t i = 0; i < MAX_CLIENTS; ++i) {
    if (this->Sockets_[i].socketType_ == CLIENT &&
        this->Sockets_[i].checkTimeout() == true) {
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

void Webserver::error_(std::string error) {
  printVerbose(error, "");
  exit(EXIT_FAILURE);
}
