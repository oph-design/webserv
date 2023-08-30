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

typedef enum eSocketType {
  SERVER,
  CLIENT,
  UNUSED
} SocketType;

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
  bool getKeepAlive() const;
  int getSocketFd();

  // setter
  void setFd(int fd);
  void setSocketIndex(int socketIndex);
  void setReqStatus();

  // other functions

  // public vars
  t_reqStatus reqStatus;

 private:
  int fd_;
  int socketIndex_;
  bool inUse;
  SocketType socketType_;
  sockaddr_in boundServerAdress_;
  int	socketOpt_;
  int listeningSocket_;
  sockaddr_in servaddr_;
  unsigned long dataSend;
  std::string response_;
  bool pendingSend;
};

bool receiveRequest(Socket &socket, size_t &bytes);

#endif  //  SOCKET_HPP
