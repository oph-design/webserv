#include "TcpServer.hpp"

void TcpServer::bootServer_() {
  Response::fillFileTypes();

  memset(&servaddr_, 0, sizeof(servaddr_));
  servaddr_.sin_family = AF_INET;
  servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr_.sin_port = htons(port_);

  listening_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listening_socket_ == -1) {
    std::cout << "socket creation failed" << std::endl;
    error_();
  }

  if (setsockopt(listening_socket_, SOL_SOCKET, SO_REUSEADDR, &socketopt_,
                 sizeof(socketopt_)) == -1) {
    std::cerr << "Error setting SO_REUSEADDR" << std::endl;
    error_();
  }

  if (bind(listening_socket_, (struct sockaddr *)&servaddr_,
           sizeof(servaddr_)) != 0) {
    std::cerr << "Could not bind socket\n";
    error_();
  }

  if (listen(listening_socket_, 25) != 0) {
    std::cerr << "Error in listen\n";
    error_();
  }

  if (fcntl(listening_socket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error setting socket to nonblocking" << std::endl;
    error_();
  }
  pollSockets_[0].setFd(listening_socket_);
  fds_[0].fd = listening_socket_;
}

void TcpServer::updateFds() {
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    fds_[i] = pollSockets_[i].getSocketPoll();
  }
}

void TcpServer::serverLoop_() {
  while (true) {
    updateFds();
    int ret = poll(fds_, nfds_, 5000);
    if (ret == -1) {
      perror("poll error");
      break;
    }
    checkPending_();
    for (int i = 0; i < nfds_; i++) {
      std::cout << "fds[" << fds_[i].fd << "].revents: " << fds_[i].revents
                << std::endl;
      if (fds_[i].revents & POLLHUP) {
        std::cout << "HANGUP " << i << " " << std::endl;
      }
      if (fds_[i].revents & POLLIN) {  // if incoming connection
        if (fds_[i].fd == listening_socket_) {
          initNewConnection_();
          break;
        } else {
          if (existingConnection_(pollSockets_[i], fds_[i], i) == false)
            closeConnection_(pollSockets_[i], fds_[i], i);
        }
      }
      if (pollSockets_[i].checkTimeout()) {
        std::cout << "Timeout of Socket: " << fds_[i].fd << std::endl;
        closeConnection_(pollSockets_[i], fds_[i], i);
      }
      pollSockets_[i].setRevent(0);
      fds_[i].revents = 0;
    }
  }
}

void TcpServer::checkPending_() {
  for (int i = 0; i < nfds_; i++) {
    if (pollSockets_[i].pendingSend == true) {
      sendResponse_(pollSockets_[i], fds_[i], i);
      return;
    }
  }
}

void TcpServer::closeConnection_(Socket &socket, pollfd &fd, int &i) {
  close(fd.fd);
  socket.closeSocket();
  socket.setInUse(false);
  for (int j = i; j < nfds_ - 1; ++j) {
    fds_[j] = fds_[j + 1];
    pollSockets_[j].setPollfd(pollSockets_[j + 1].getSocketPoll());
  }
  --nfds_;
  --i;
}

void TcpServer::initNewConnection_() {
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = sizeof(clientaddr);
  int new_client_sock = accept(listening_socket_,
                               (struct sockaddr *)&clientaddr, &clientaddr_len);
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error setting socket to nonblocking" << std::endl;
    error_();
  }
  pollSockets_[nfds_].setFd(new_client_sock);
  pollSockets_[nfds_].setTimestamp();
  pollSockets_[nfds_].setInUse(true);
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
  size_t bytes_read = 0;

  bytes_read = recv(socket.getSocketFd(), buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    std::cout << "connection established with socket " << socket.getSocketFd()
              << " " << std::endl;
    socket.setTimestamp();
    if (socket.pendingSend == false) {
      socket.response_ = createResponse_(buffer);
    }
    sendResponse_(socket, fd, i);
  }
  if (bytes_read == (size_t)-1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
    std::cout << "BLOCKER: " << socket.getSocketFd() << std::endl;
  } else if (bytes_read == 0) {
    std::cout << "client closed connection on socket " << socket.getSocketFd()
              << " " << std::endl;
    return false;
  }
  return true;
}

bool TcpServer::isKeepAlive(const Socket &socket) {
  return socket.getKeepAlive();
}

void TcpServer::error_() {
  std::cerr << "ERROR" << std::endl;
  exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
void TcpServer::boot() {
  bootServer_();
  serverLoop_();
}

TcpServer::TcpServer(std::string ip_addr, int port)
    : ip_addr_(ip_addr), port_(port), nfds_(1), socketopt_(1) {}

TcpServer::TcpServer(const TcpServer &rhs) { (void)rhs; }
TcpServer::~TcpServer() {}

TcpServer &TcpServer::operator=(const TcpServer &rhs) {
  (void)rhs;
  return *this;
}
