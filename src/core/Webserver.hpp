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
  Webserver(std::string ip_addr, int port);
  Webserver(Config &config, int port);
  Webserver(const Webserver &);
  ~Webserver();
  Webserver &operator=(const Webserver &);

  void boot();

 private:
	int	socketNum_;
	int serverSocketNum_;
	int clientSocketNum_;
	int socketOpt_;
	struct pollfd fds_[MAX_CLIENTS];
	Socket Sockets_[MAX_CLIENTS];

	void createServerSocket_(Socket &, int);

};

#endif  // WEBSERVER_HPP
