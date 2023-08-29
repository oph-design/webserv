#include "Webserver.hpp"

Webserver::Webserver(const Webserver &rhs) { *this = rhs; }

Webserver::~Webserver() {}

Webserver::Webserver(int serverNum, std::string ipAddr, int port)
    : socketNum_(0), serverSocketNum_(0), clientSocketNum_(0) {
  serverSocketNum_ = 1;
  /* ipAddr = "localhost";
  port = 1234; */
	for(size_t i = 0; i < MAX_CLIENTS; i++){
		fds_[i].fd = -1;
		fds_[i].events = POLLIN;
		fds_[i].revents = 0;
	}
  createServerSocket(Sockets_[0], port);
}

void Webserver::createServerSocket_(Socket &serverSocket, int port) {
  
	memset(serverSocket.servaddr_, 0, sizeof(serverSocket.servaddr_));
  serverSocket.servaddr_.sin_family = AF_INET;
  serverSocket.servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSocket.servaddr_.sin_port = htons(port);

  serverSocket.listeningSocket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket.listeningSocket_ == -1) {
    std::cerr << "Error: Socket creation failed" << std::endl;
    error_();
  }

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEADDR,
                 1, sizeof(this->socketopt_)) == -1) {
    std::cerr << "Error: Setting SO_REUSEADDR" << std::endl;
    error_();
  }

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEPORT,
                 serverSocket.socketOpt_, sizeof(serverSocket.socketOpt_)) == -1) {
    std::cerr << "Error: Setting SO_REUSEADDR" << std::endl;
    error_();
  }

  if (bind(serverSocket.listeningSocket_, (struct sockaddr *)serverSocket.servaddr_,
           sizeof(serverSocket.servaddr_)) != 0) {
    std::cerr << "Error: Could not bind socket" << std::endl;
    error_();
  }

  if (listen(serverSocket.listeningSocket_, 25) != 0) {
    std::cerr << "Error: Listen" << std::endl;
    error_();
  }

  if (fcntl(serverSocket.listeningSocket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error: Setting socket to nonblocking" << std::endl;
    error_();
  }

	this->socketNum_++;
  this->serverSocketNum++;
}