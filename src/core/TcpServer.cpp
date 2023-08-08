#include "TcpServer.hpp"

void TcpServer::_bootServer() {
  Response::fillFileTypes();
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

  if (fcntl(_listening_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    _error();
  }

  memset(&_servaddr, 0, sizeof(_servaddr));
  _servaddr.sin_family = AF_INET;
  _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _servaddr.sin_port = htons(_port);

  if (bind(_listening_socket, (struct sockaddr *)&_servaddr,
           sizeof(_servaddr)) != 0) {
    std::cerr << "Could not bind socket\n";
    _error();
  }

  if (listen(_listening_socket, 5) != 0) {
    std::cerr << "Error in listen\n";
    _error();
  }

  pollSockets_[0].setFd(_listening_socket);
}

void TcpServer::updateFds() {
  for (size_t i = 0; i < MAX_CLIENTS; i++) {
    _fds[i] = pollSockets_[i].getSocketPoll();
  }
}

void TcpServer::_serverLoop() {
  while (true) {
    updateFds();
    int ret = poll(_fds, _nfds, 50000);
    if (ret == -1) {
      std::cout << "poll error" << std::endl;
      break;
    }
		std::cout << "BEFORE CHECK PENDING" << std::endl;
		checkPending_();
		std::cout << "AFTER CHECK PENDING" << std::endl;
    for (int i = 0; i < _nfds; i++) {
      std::cout << "fds[" << i << "].revents: " << _fds[i].revents << std::endl;
      if (_fds[i].revents & POLLHUP) {
        std::cout << "HANGUP " << i << " " << std::endl;
      }
      if (_fds[i].revents & POLLIN) {  // if incoming connection
        if (_fds[i].fd == _listening_socket) {
          _initNewConnection();
        } else {
          if (_existingConnection(pollSockets_[i]) == false)
            closeConnection_(pollSockets_[i], _fds[i], i);
        }
        if (pollSockets_[i].checkTimeout()) {
          std::cout << "Timeout of Socket: " << _fds[i].fd << std::endl;
          closeConnection_(pollSockets_[i], _fds[i], i);
        }
      }
    }
  }
}

void TcpServer::checkPending_(){
	for(int i = 0; i < _nfds; i++){
		if(pollSockets_[i].pendingSend == true){
			sendResponse_(pollSockets_[i]);
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

void TcpServer::sendResponse_(Socket &socket){
	std::cout << "BEFORE" << std::endl;
	socket.dataSend = send(socket.getSocketFd(), socket.response_.c_str(),
													socket.response_.size(), 0);
	if (socket.dataSend < socket.response_.size()) {
		socket.pendingSend = true;
		socket.response_ = socket.response_.substr(socket.dataSend);
	} else
		socket.pendingSend = false;
	std::cout << "AFTER" << std::endl;
}

bool TcpServer::_existingConnection(Socket &socket) {
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
    sendResponse_(socket);
  } else if (bytes_read == 0 || !isKeepAlive(socket)) {
    std::cout << "client closed connection on socket " << socket.getSocketFd()
              << " " << std::endl;
    return false;
  }
  return true;
}

/* void TcpServer::_existingConnection(int &i) {
  char buffer[1024] = {0};
  size_t bytes_read = 0;

  bytes_read = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    Request request(buffer);
    Response resobj(request);
    pollSockets_[i].setTimestamp();
    std::cout << "connection established with socket " << _fds[i].fd << " "
              << std::endl;
    std::string response = resobj.getHeader() + resobj.getFullBody();
    std::cout << response << std::endl;              //// RESPONSE PRINT
    std::string header = resobj.getHeader();         //// HEADER
    std::list<std::string> body = resobj.getBody();  /// BODY LIST

                std::cout << "BYTES SEND: " << send(_fds[i].fd,
response.c_str(), response.size(), 0) << std::endl; std::cout << "CONTENT
LENGTH: " << response.size() << std::endl;
                //send(_fds[i].fd, header.c_str(), header.size(), 0);
    //send(_fds[i].fd, body.front().c_str(), body.front().size(), 0);

    if (body.size() == 1 && pollSockets_[i].pendingSend == false) {
      send(_fds[i].fd, header.c_str(), header.size(), 0);
      send(_fds[i].fd, body.front().c_str(), body.front().size(), 0);
    } else {
                                handleSegmentedTransmission(i, body, header);
    }
  } else if (bytes_read == 0 || !isKeepAlive(pollSockets_[i])) {
    std::cout << "client closed connection on socket " << _fds[i].fd << " "
              << std::endl;
    closeConnection_(pollSockets_[i], _fds[i], i);
  }
}

void TcpServer::handleSegmentedTransmission(int &i, std::list<std::string>
body, std::string header){ if (pollSockets_[i].pendingSend == false) {
                        pollSockets_[i].response_ = body;
                        pollSockets_[i].it = pollSockets_[i].response_.begin();
                        send(_fds[i].fd, header.c_str(), header.size(), 0);
                        send(_fds[i].fd, pollSockets_[i].it->c_str(),
                                                pollSockets_[i].it->size(), 0);
                        pollSockets_[i].it++;
                        pollSockets_[i].pendingSend = true;
                        return ;
                } else if (pollSockets_[i].it ==
pollSockets_[i].response_.end()) { pollSockets_[i].pendingSend = false; } else
if (pollSockets_[i].pendingSend == true) { send(_fds[i].fd,
pollSockets_[i].it->c_str(), pollSockets_[i].it->size(), 0);
                        pollSockets_[i].it++;
                }
}

void TcpServer::sendFile_(int fd, std::list<std::string> body) {
   size_t totalSend = 0;
    while (totalSend < response.size()) {
      size_t toSend =
          std::min(response.size() - totalSend, static_cast<size_t>(512));
                  size_t sent = send(fd, response.data() + totalSend, toSend,
    0); if(sent < 0){ std::cerr << "Error in response send" << std::endl; return
    ;
                  }
                  totalSend += sent;
    }
  for (std::list<std::string>::iterator it = body.begin(); it != body.end();
       it++) {
    // std::cout << *it << std::endl;
    send(fd, it->c_str(), it->size(), 0);
    usleep(2000);
  }
} */

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
