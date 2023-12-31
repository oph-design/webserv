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

#include "Utils.hpp"

typedef enum eSocketType { SERVER, CLIENT, UNUSED } SocketType;

typedef struct s_reqStatus {
  bool pendingReceive;
  int clen;
  int readBytes;
  std::string buffer;
} t_reqStatus;

class Socket {
  friend class Webserver;

 public:
  Socket();
  ~Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);

  // getter
  const bool &getKeepAlive() const;
  const int &getFd() const;

  // setter
  void setReqStatus();
  void setIdle();
  void setTimestamp();

  // other functions
  bool checkTimeout();

  // public vars
  t_reqStatus reqStatus;

 private:
  int fd_;
  int socketIndex_;
  SocketType socketType_;
  int socketOpt_;
  int listeningSocket_;
  sockaddr_in socketaddr_;
  unsigned long dataSend_;
  std::string response_;
  bool pendingSend_;
  bool keepAlive_;
  time_t timestamp_;
  double timeout_;
};

bool receiveRequest(Socket &socket, size_t &bytes);

#endif  //  SOCKET_HPP
