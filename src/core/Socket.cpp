#include "Socket.hpp"

// public
Socket::Socket() : fd_(-1), socketIndex_(-1), socketType_(UNUSED), socketOpt_(1) {}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
  return *this;
}

// getter

// setter

void Socket::setFd(int fd) { this->fd_ = fd; }
void Socket::setSocketIndex(int index) { this->socketIndex_ = index; }

// other functions

// private
