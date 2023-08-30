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
  Webserver(int port);
  Webserver(Config &config, int port);
  Webserver(const Webserver &);
  ~Webserver();
  Webserver &operator=(const Webserver &);

  void boot();

 private:
    size_t	socketNum_;
    size_t serverSocketNum_;
    size_t clientSocketNum_;
	int socketOpt_;
	struct pollfd fds_[MAX_CLIENTS];
	Socket Sockets_[MAX_CLIENTS];

	void createServerSocket_(Socket &, int);
    void createClientSocket_(Socket &);
    void startServerRoutine_();
    void error_(std::string);
    int getServerListen();
    void sendResponse_(Socket &socket, pollfd &fd, size_t &i);
    bool existingConnection_(Socket &socket, pollfd &fd, size_t &i);
    std::string createResponse_(std::string buffer);
    void closeConnection_(Socket &socket, pollfd &fd, size_t &i);
    void checkPending_();

};

#endif  // WEBSERVER_HPP
