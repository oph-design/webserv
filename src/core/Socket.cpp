#include "Socket.hpp"

// public
Socket::Socket()
    : pendingSend(false),
      keepAlive_(true),
      socketOpt_(1),
      timestamp_(0),
      timeout_(5.0),
      inUse_(false) {
  socketFd_.fd = -1;
  socketFd_.events = POLLIN;
  socketFd_.revents = 0;
}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
  timestamp_ = rhs.timestamp_;
  keepAlive_ = rhs.keepAlive_;
  socketOpt_ = rhs.socketOpt_;
  socketFd_ = rhs.socketFd_;
  return *this;
}

// getter

int Socket::getRevents() const { return socketFd_.revents; }

struct pollfd Socket::getSocketPoll() const { return socketFd_; }

bool Socket::getKeepAlive() const { return keepAlive_; }

int	Socket::getSocketFd(){
	return socketFd_.fd;
}

// setter

void Socket::setFd(int fd) { socketFd_.fd = fd; }

void Socket::setPollfd(const struct pollfd rhs) { this->socketFd_ = rhs; }

void Socket::setTimestamp() { this->timestamp_ = std::time(NULL); }

void Socket::setInUse(bool set) { inUse_ = set; }

// other functions
bool Socket::checkTimeout() {
  if (this->inUse_) {
    time_t current = std::time(NULL);
    if (difftime(current, this->timestamp_) >= timeout_) return true;
  }
  return false;
}

void Socket::closeSocket() { close(socketFd_.fd); }

// private
