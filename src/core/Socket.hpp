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
#include <list>
#include <sstream>
#include <string>

typedef struct s_reqStatus {
  bool pendingReceive;
  int clen;
  int readBytes;
  std::string buffer;
} t_reqStatus;

class Socket {
 public:
  Socket();
  ~Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);

  // getter
  int getRevents(void) const;
  struct pollfd getSocketPoll() const;
  bool getKeepAlive() const;
  int getSocketFd();

  // setter
  void setFd(int fd);
  void setPollfd(const struct pollfd);
  void setTimestamp();
  void setInUse(bool);
  void setRevent(int);
  void setEvent(int);
  void setKeepAlive(bool);
  void setReqStatus();

  // other functions
  bool checkTimeout();
  void closeSocket();

  // public vars
  std::string response_;
  std::list<std::string>::iterator it;
  bool pendingSend;
  size_t dataSend;
  t_reqStatus reqStatus;

 private:
  struct pollfd socketFd_;
  bool keepAlive_;
  int socketOpt_;
  time_t timestamp_;
  double timeout_;
  bool inUse_;
};

bool receiveRequest(Socket &socket, size_t &bytes);

#endif  //  SOCKET_HPP
