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

#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 131072

class TcpServer {
  friend class ServerCluster;

 public:
  TcpServer(std::string ip_addr, int port);
  TcpServer(Config &config);
  TcpServer(const TcpServer &);
  ~TcpServer();
  TcpServer &operator=(const TcpServer &);

  void boot();

 private:
  void bootServer_();
  bool serverLoop_();
  void initNewConnection_();
  bool existingConnection_(Socket &, pollfd &, int &);
  void sendFile_(int, std::list<std::string>);
  bool isKeepAlive_(const Socket &socket);
  void closeConnection_(Socket &socket, pollfd &fd, int &i);
  std::string createResponse_(std::string buffer);
  void sendResponse_(Socket &, pollfd &, int &);
  void checkPending_();
  void error_();
  void updateFds_();
  void handleSegmentedTransmission_(int &, std::list<std::string>, std::string);

  int listening_socket_;
  sockaddr_in servaddr_;
  struct pollfd fds_[MAX_CLIENTS];
  Socket pollSockets_[MAX_CLIENTS];
  std::string ip_addr_;
  int port_;
  int nfds_;
  int socketopt_;

  Config config_;
};

#endif  // TCPSERVER_HPP
