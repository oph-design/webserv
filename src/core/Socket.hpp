#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Socket {
 public:
  Socket();
  ~Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);
  // getter
  int getRevents(void) const;
  struct pollfd getSocketFd() const;
  bool getKeepAlive() const;
  // setter
  void setFd(int fd);
  void setPollfd(const struct pollfd);
  void setTimestamp();
  void setInUse(bool);
  // other functions
  bool checkTimeout();
  void closeSocket();

 private:
  struct pollfd socketFd_;
  bool keepAlive_;
  int socketOpt_;
  time_t timestamp_;
  double timeout_;
  bool inUse_;
};

#endif  //  SOCKET_HPP
