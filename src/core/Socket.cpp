#include "Socket.hpp"

// public
Socket::Socket()
    : pendingSend(false),
      keepAlive_(true),
      socketOpt_(1),
      timestamp_(0),
      timeout_(5.0),
      inUse_(false) {
  this->socketFd_.fd = -1;
  this->socketFd_.events = POLLIN;
  this->socketFd_.revents = 0;
  this->setReqStatus();
}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
  this->timestamp_ = rhs.timestamp_;
  this->keepAlive_ = rhs.keepAlive_;
  this->socketOpt_ = rhs.socketOpt_;
  this->socketFd_ = rhs.socketFd_;
  this->timeout_ = rhs.timeout_;
  this->inUse_ = rhs.inUse_;
  return *this;
}

// getter

int Socket::getRevents() const { return this->socketFd_.revents; }

struct pollfd Socket::getSocketPoll() const { return this->socketFd_; }

bool Socket::getKeepAlive() const { return this->keepAlive_; }

int Socket::getSocketFd() { return this->socketFd_.fd; }

// setter

void Socket::setFd(int fd) { this->socketFd_.fd = fd; }

void Socket::setPollfd(const struct pollfd rhs) { this->socketFd_ = rhs; }

void Socket::setTimestamp() { this->timestamp_ = std::time(NULL); }

void Socket::setInUse(bool set) { this->inUse_ = set; }

void Socket::setRevent(int revent) { this->socketFd_.revents = revent; }

void Socket::setEvent(int event) { this->socketFd_.events = event; }

void Socket::setKeepAlive(bool set) { this->keepAlive_ = set; }

void Socket::setReqStatus() {
  this->reqStatus.pendingReceive = false;
  this->reqStatus.chunked = false;
  this->reqStatus.clen = 0;
  this->reqStatus.readBytes = 0;
}
// other functions
bool Socket::checkTimeout() {
  if (this->inUse_) {
    time_t current = std::time(NULL);
    if (difftime(current, this->timestamp_) >= this->timeout_) return true;
  }
  return false;
}

void Socket::closeSocket() { close(this->socketFd_.fd); }

// private
