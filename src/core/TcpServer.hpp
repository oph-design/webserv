#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define MAX_CLIENTS 1024

class TcpServer {
 public:
  TcpServer(std::string ip_addr, int port);
  TcpServer(const TcpServer &);
  ~TcpServer();
  TcpServer &operator=(const TcpServer &);
  void boot();

 private:
  void _bootServer();
  void _serverLoop();
  void _initNewConnection();
  void _existingConnection(int &);
	bool isKeepAlive();
  std::string _createResponse();
  void _error();

  int _listening_socket;
  sockaddr_in _servaddr;
  struct pollfd _fds[MAX_CLIENTS];
  std::string _ip_addr;
  int _port;
  int _nfds;
  int _socketopt;
};

#endif  // TCPSERVER_HPP
