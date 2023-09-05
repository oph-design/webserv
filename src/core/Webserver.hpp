#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

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

class Webserver {
 public:
  Webserver(ConfigVector &);
  Webserver(const Webserver &);
  ~Webserver();
  Webserver &operator=(const Webserver &);

  void boot();

 private:
  void createServerSocket_(Socket &, int, double);
  void createClientSocket_(Socket &);
  void startServerRoutine_();
  void error_(std::string);
  void sendResponse_(Socket &socket, pollfd &fd, size_t &i);
  bool existingConnection_(Socket &socket, pollfd &fd, size_t &i);
  std::string createResponse_(Socket &socket);
  void closeConnection_(Socket &socket, pollfd &fd, size_t &i);
  void checkPending_();
  void checkTimeoutClients();
  int getConfigId_(int tofind);

  size_t socketNum_;
  size_t serverSocketNum_;
  size_t clientSocketNum_;
  int socketOpt_;
  ConfigVector &configs_;
  struct pollfd fds_[MAX_CLIENTS];
  Socket Sockets_[MAX_CLIENTS];
};

#endif  // WEBSERVER_HPP
