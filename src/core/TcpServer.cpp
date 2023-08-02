#include "TcpServer.hpp"

#include <string>

void TcpServer::_bootServer() {
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

  _fds[0].fd = _listening_socket;
  _fds[0].events = POLLIN;
  _fds[0].revents = 0;
}

void TcpServer::_serverLoop() {
  while (true) {
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
      }
    }
  }
}

void TcpServer::_initNewConnection() {
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = sizeof(clientaddr);
  int new_client_sock = accept(_listening_socket,
                               (struct sockaddr *)&clientaddr, &clientaddr_len);
  _fds[_nfds].fd = new_client_sock;
  _fds[_nfds].events = POLLIN;
  _fds[_nfds].revents = 0;
  ++_nfds;
}

void TcpServer::_existingConnection(int &i) {
  char buffer[1024] = {0};
  size_t bytes_read = 0;
  bytes_read = read(_fds[i].fd, buffer, sizeof(buffer));
  if (bytes_read > 0) {
    std::cout << "connection established with socket " << i << " " << std::endl;
    std::string response = _createResponse();
    write(_fds[i].fd, response.c_str(), response.size());
  } else {
    std::cout << "client closed connection on socket " << i << " " << std::endl;
    close(_fds[i].fd);
    for (int j = i; j < _nfds - 1; ++j) {
      _fds[j] = _fds[j + 1];
    }
    --_nfds;
    --i;
  } 
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
      "Content-Type: text/plain; charset=UTF-8\r\n"
      "Connection: keep-alive\r\n"
      "Content-Length: ";
  std::string final_response(httpResponse);
  final_response.append(std::to_string(html_content.str().length()));
  final_response.append("\r\n\r\n");
  final_response.append(html_content.str());
  return final_response;
}

void TcpServer::_error() { return; }

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
