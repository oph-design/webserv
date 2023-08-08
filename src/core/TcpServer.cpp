#include "TcpServer.hpp"

void TcpServer::_bootServer() {
  Response::fillFileTypes();

  memset(&_servaddr, 0, sizeof(_servaddr));
  _servaddr.sin_family = AF_INET;
  _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _servaddr.sin_port = htons(_port);

	memset(this->_fds, -1, (4 * MAX_CLIENTS));

	  _listening_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_listening_socket == -1) {
    std::cout << "socket creation failed" << std::endl;
    _error();
  }

  if (setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, &_socketopt,
                 sizeof(_socketopt)) == -1) {
    std::cerr << "Error setting SO_REUSEADDR" << std::endl;
    _error();
  }

	if (bind(_listening_socket, (struct sockaddr *)&_servaddr,
           sizeof(_servaddr)) != 0) {
    std::cerr << "Could not bind socket\n";
    _error();
  }

	  if (listen(_listening_socket, 25) != 0) {
    std::cerr << "Error in listen\n";
    _error();
  }

	  if (fcntl(_listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error setting socket to nonblocking" << std::endl;
    _error();
  }
  pollSockets_[0].setFd(_listening_socket);
	_fds[0].fd = _listening_socket;
}

void TcpServer::updateFds() {
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    _fds[i] = pollSockets_[i].getSocketPoll();
  }
}

void TcpServer::_serverLoop() {
  while (true) {
    updateFds();
		std::cout << "before" << std::endl;
    int ret = poll(_fds, _nfds, 5000);
		std::cout << "after" << std::endl;
    if (ret == -1) {
      perror("poll error");
      break;
    }
		checkPending_();
    for (int i = 0; i < _nfds; i++) {
      std::cout << "fds[" << _fds[i].fd << "].revents: " << _fds[i].revents << std::endl;
      if (_fds[i].revents & POLLHUP) {
        std::cout << "HANGUP " << i << " " << std::endl;
      }
      if (_fds[i].revents & POLLIN) {  // if incoming connection
        if (_fds[i].fd == _listening_socket) {
          _initNewConnection();
					break;
        } else {
          if (_existingConnection(pollSockets_[i], _fds[i], i) == false)
            closeConnection_(pollSockets_[i], _fds[i], i);
        }
/*         if (pollSockets_[i].checkTimeout()) {
          std::cout << "Timeout of Socket: " << _fds[i].fd << std::endl;
          closeConnection_(pollSockets_[i], _fds[i], i);
        } */
      }
			pollSockets_[i].setRevent(0);
			_fds[i].revents = 0;
    }
  }
}

void TcpServer::checkPending_(){
	for(int i = 0; i < _nfds; i++){
		if(pollSockets_[i].pendingSend == true){
			sendResponse_(pollSockets_[i], _fds[i], i);
			return ;
		}
	}
}

void TcpServer::closeConnection_(Socket &socket, pollfd &fd, int &i) {
  close(fd.fd);
  socket.closeSocket();
  socket.setInUse(false);
  for (int j = i; j < _nfds - 1; ++j) {
    _fds[j] = _fds[j + 1];
    pollSockets_[j].setPollfd(pollSockets_[j + 1].getSocketPoll());
  }
  --_nfds;
  --i;
}

void TcpServer::_initNewConnection() {
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = sizeof(clientaddr);
  int new_client_sock = accept(_listening_socket,
                               (struct sockaddr *)&clientaddr, &clientaddr_len);
	if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error setting socket to nonblocking" << std::endl;
    _error();
  }
  pollSockets_[_nfds].setFd(new_client_sock);
  pollSockets_[_nfds].setTimestamp();
  pollSockets_[_nfds].setInUse(true);
  ++_nfds;
}

std::string TcpServer::_createResponse(char buffer[1024]) {
  Request request(buffer);
  Response resobj(request);
  std::string response = resobj.getHeader() + resobj.getFullBody();
  return response;
}

void TcpServer::sendResponse_(Socket &socket, pollfd &fd, int &i){
	socket.dataSend = send(socket.getSocketFd(), socket.response_.c_str(),
													socket.response_.size(), 0);
	if (socket.dataSend < socket.response_.size()) {
		socket.pendingSend = true;
		socket.response_ = socket.response_.substr(socket.dataSend);
	} else{
		socket.pendingSend = false;
		closeConnection_(socket, fd, i);
	}
}

bool TcpServer::_existingConnection(Socket &socket, pollfd &fd, int &i) {
  char buffer[1024] = {0};
  size_t bytes_read = 0;

  bytes_read = recv(socket.getSocketFd(), buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    std::cout << "connection established with socket " << socket.getSocketFd()
              << " " << std::endl;
    socket.setTimestamp();
		if (socket.pendingSend == false) {
			socket.response_ = _createResponse(buffer);
		}
    sendResponse_(socket, fd, i);
  }
	if (bytes_read == (size_t)-1 && (errno == EWOULDBLOCK || errno == EAGAIN)){
		std::cout << "BLOCKER: " << socket.getSocketFd() << std::endl;
		}
	 else if (bytes_read == 0) {
    std::cout << "client closed connection on socket " << socket.getSocketFd()
              << " " << std::endl;
    return false;
  }
  return true;
}

bool TcpServer::isKeepAlive(const Socket &socket) {
  return socket.getKeepAlive();
}

void TcpServer::_error() {
  std::cerr << "ERROR" << std::endl;
  exit(EXIT_FAILURE);
}

////////////////////////////////////////////////
void TcpServer::boot() {
  _bootServer();
  _serverLoop();
}

TcpServer::TcpServer(std::string ip_addr, int port)
    : _ip_addr(ip_addr), _port(port), _nfds(1), _socketopt(1) {}

TcpServer::TcpServer(const TcpServer &rhs) { (void)rhs; }
TcpServer::~TcpServer() {}

TcpServer &TcpServer::operator=(const TcpServer &rhs) {
  (void)rhs;
  return *this;
}
