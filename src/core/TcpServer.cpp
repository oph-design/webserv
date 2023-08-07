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
    _fds[i] = pollSockets_[i].getSocketFd();
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
    for (int i = 0; i < _nfds; i++) {
      std::cout << "fds[" << i << "].revents: " << _fds[i].revents << std::endl;
      if (_fds[i].revents & POLLHUP) {
        std::cout << "HANGUP " << i << " " << std::endl;
      }
      if (_fds[i].revents & POLLIN) {  // if incoming connection
        if (_fds[i].fd == _listening_socket) {
          _initNewConnection();
        } else {
          _existingConnection(i);
        }
        if (pollSockets_[i].checkTimeout()) {
          std::cout << "Timeout of Socket: " << _fds[i].fd << std::endl;
          closeConnection_(pollSockets_[i], _fds[i], i);
        }
      }
    }
  }
}

void TcpServer::closeConnection_(Socket &socket, pollfd &fd, int &i) {
  close(fd.fd);
  socket.closeSocket();
  socket.setInUse(false);
  for (int j = i; j < _nfds - 1; ++j) {
    _fds[j] = _fds[j + 1];
    pollSockets_[j].setPollfd(pollSockets_[j + 1].getSocketFd());
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

void TcpServer::_existingConnection(int &i) {
  char buffer[1024] = {0};
  size_t bytes_read = 0;

  bytes_read = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    Request request(buffer);
    Response resobj(request);
    pollSockets_[i].setTimestamp();
    std::cout << "connection established with socket " << _fds[i].fd << " "
              << std::endl;
    std::string response = resobj.getHeader() + resobj.getBody().front();
    std::cout << response << std::endl;
    std::string header = resobj.getHeader();
    std::list<std::string> body = resobj.getBody();
    send(_fds[i].fd, header.c_str(), header.size(), 0);
    sendFile_(_fds[i].fd, body);
  } else if (bytes_read == 0 || !isKeepAlive(pollSockets_[i])) {
    std::cout << "client closed connection on socket " << _fds[i].fd << " "
              << std::endl;
    closeConnection_(pollSockets_[i], _fds[i], i);
  }
}

void TcpServer::sendFile_(int fd, std::list<std::string> body) {
  /*   size_t totalSend = 0;
    while (totalSend < response.size()) {
      size_t toSend =
          std::min(response.size() - totalSend, static_cast<size_t>(512));
                  size_t sent = send(fd, response.data() + totalSend, toSend,
    0); if(sent < 0){ std::cerr << "Error in response send" << std::endl; return
    ;
                  }
                  totalSend += sent;
    } */
  for (std::list<std::string>::iterator it = body.begin(); it != body.end();
       it++) {
    // std::cout << *it << std::endl;
    send(fd, it->c_str(), it->size(), 0);
  }
}

bool TcpServer::isKeepAlive(const Socket &socket) {
  return socket.getKeepAlive();
}

std::string TcpServer::_createResponse() {
  std::ifstream htmlFile("html/index.html");
  if (htmlFile.is_open()) {
    std::cout << "html File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open the html file." << std::endl;
  }
  std::stringstream html_content;
  html_content << htmlFile.rdbuf();
  const char *httpResponse =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: ";
  std::string final_response(httpResponse);
  final_response.append(std::to_string(html_content.str().length()));
  final_response.append("\r\n\r\n");
  final_response.append(html_content.str());
  return final_response;
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
