#include "TcpServer.hpp"

void TcpServer::bootServer_() {
  memset(&this->servaddr_, 0, sizeof(this->servaddr_));
  this->servaddr_.sin_family = AF_INET;
  this->servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  this->servaddr_.sin_port = htons(port_);

  this->listening_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (this->listening_socket_ == -1) {
    std::cerr << "Error: Socket creation failed" << std::endl;
    error_();
  }

  if (setsockopt(this->listening_socket_, SOL_SOCKET, SO_REUSEADDR,
                 &this->socketopt_, sizeof(this->socketopt_)) == -1) {
    std::cerr << "Error: Setting SO_REUSEADDR" << std::endl;
    error_();
  }

  if (setsockopt(this->listening_socket_, SOL_SOCKET, SO_REUSEPORT,
                 &this->socketopt_, sizeof(this->socketopt_)) == -1) {
    std::cerr << "Error: Setting SO_REUSEADDR" << std::endl;
    error_();
  }

  if (bind(this->listening_socket_, (struct sockaddr *)&this->servaddr_,
           sizeof(this->servaddr_)) != 0) {
    std::cerr << "Error: Could not bind socket" << std::endl;
    error_();
  }

  if (listen(this->listening_socket_, 25) != 0) {
    std::cerr << "Error: Listen" << std::endl;
    error_();
  }

  if (fcntl(this->listening_socket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error: Setting socket to nonblocking" << std::endl;
    error_();
  }
  this->pollSockets_[0].setFd(this->listening_socket_);
  this->fds_[0].fd = this->listening_socket_;
}

void TcpServer::updateFds_() {
  for (std::size_t i = 0; i < MAX_CLIENTS; ++i) {
    this->fds_[i] = this->pollSockets_[i].getSocketPoll();
  }
}

bool TcpServer::serverLoop_() {
  this->updateFds_();
  int ret = poll(this->fds_, this->nfds_, 1);
  if (ret == -1) {
    perror("poll error");
    return true;
  }
  this->checkPending_();
  for (int i = 0; i < this->nfds_; ++i) {
    // std::cout << "fds[" << this->fds_[i].fd
    //           << "].revents: " << this->fds_[i].revents << std::endl;
    if (this->fds_[i].revents & POLLHUP) {
      std::cout << "HANGUP " << i << " " << std::endl;
    }
    if (this->fds_[i].revents & POLLIN) {
      if (this->fds_[i].fd == this->listening_socket_) {
        this->initNewConnection_();
        return true;
      } else {
        if (!this->existingConnection_(this->pollSockets_[i], this->fds_[i], i))
          this->closeConnection_(this->pollSockets_[i], this->fds_[i], i);
      }
    }
    if (this->pollSockets_[i].checkTimeout()) {
      std::cout << "Timeout of Socket: " << this->fds_[i].fd << std::endl;
      this->closeConnection_(this->pollSockets_[i], this->fds_[i], i);
    }
    this->pollSockets_[i].setRevent(0);
    this->fds_[i].revents = 0;
  }
  return false;
}

void TcpServer::checkPending_() {
  for (int i = 0; i < nfds_; ++i) {
    if (this->pollSockets_[i].pendingSend == true) {
      this->sendResponse_(this->pollSockets_[i], fds_[i], i);
      return;
    }
  }
}

void TcpServer::closeConnection_(Socket &socket, pollfd &fd, int &i) {
  close(fd.fd);
  socket.closeSocket();
  socket.setInUse(false);
  for (int j = i; j < nfds_ - 1; ++j) {
    this->fds_[j] = this->fds_[j + 1];
    this->pollSockets_[j].setPollfd(this->pollSockets_[j + 1].getSocketPoll());
  }
  --nfds_;
  --i;
}

void TcpServer::initNewConnection_() {
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = sizeof(clientaddr);
  int new_client_sock = accept(this->listening_socket_,
                               (struct sockaddr *)&clientaddr, &clientaddr_len);
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error: Setting socket to nonblocking" << std::endl;
    error_();
  }
  this->pollSockets_[nfds_].setFd(new_client_sock);
  this->pollSockets_[nfds_].setTimestamp();
  this->pollSockets_[nfds_].setInUse(true);
  ++nfds_;
}

std::string TcpServer::createResponse_(char buffer[1024]) {
  Request request(buffer);
  Response resobj(request);
  std::string response = resobj.getHeader() + resobj.getBody();
  return response;
}

void TcpServer::sendResponse_(Socket &socket, pollfd &fd, int &i) {
  socket.dataSend = send(socket.getSocketFd(), socket.response_.c_str(),
                         socket.response_.size(), 0);
  if (socket.dataSend < socket.response_.size()) {
    socket.pendingSend = true;
    socket.response_ = socket.response_.substr(socket.dataSend);
    socket.setEvent(POLLOUT);
  } else {
    socket.pendingSend = false;
    socket.setEvent(POLLIN);
    if (!socket.getKeepAlive()) closeConnection_(socket, fd, i);
  }
}

bool TcpServer::existingConnection_(Socket &socket, pollfd &fd, int &i) {
  char buffer[BUFFER_SIZE] = {0};
  std::size_t bytes_read = 0;

  bytes_read = recv(socket.getSocketFd(), buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    std::cout << "connection on socket " << socket.getSocketFd() << std::endl;
    socket.setTimestamp();
    if (socket.pendingSend == false) {
      socket.response_ = this->createResponse_(buffer);
    }
    this->sendResponse_(socket, fd, i);
  }
  if (bytes_read == static_cast<std::size_t>(-1) &&
      (errno == EWOULDBLOCK || errno == EAGAIN)) {
    std::cout << "BLOCKER: " << socket.getSocketFd() << std::endl;
  } else if (bytes_read == 0) {
    std::cout << "client closed socket " << socket.getSocketFd() << std::endl;
    return false;
  }
  return true;
}

bool TcpServer::isKeepAlive_(const Socket &socket) {
  return socket.getKeepAlive();
}

void TcpServer::error_() {
  std::cerr << "ERROR" << std::endl;
  exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
void TcpServer::boot() {
  this->bootServer_();
  this->serverLoop_();
}

TcpServer::TcpServer(std::string ip_addr, int port)
    : ip_addr_(ip_addr), port_(port), nfds_(1), socketopt_(1) {}

TcpServer::TcpServer(Config &config, int port)
    : ip_addr_(config.getRoot()),
      port_(port),
      nfds_(1),
      socketopt_(1),
      config_(config) {}

TcpServer::TcpServer(const TcpServer &rhs) { *this = rhs; }
TcpServer::~TcpServer() {}

TcpServer &TcpServer::operator=(const TcpServer &rhs) {
  this->listening_socket_ = rhs.listening_socket_;
  this->servaddr_ = rhs.servaddr_;
  for (int i = 0; i < MAX_CLIENTS; ++i) {
    this->fds_[i] = rhs.fds_[i];
    this->pollSockets_[i] = rhs.pollSockets_[i];
  }
  this->ip_addr_ = rhs.ip_addr_;
  this->port_ = rhs.port_;
  this->nfds_ = rhs.nfds_;
  this->socketopt_ = rhs.socketopt_;
  this->config_ = rhs.config_;
  return *this;
}
