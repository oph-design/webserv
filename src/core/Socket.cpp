#include "Socket.hpp"

// public
Socket::Socket() : fd_(-1), socketIndex_(-1), socketType_(UNUSED), socketOpt_(1) {}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
    (void)rhs;
    return *this;
}

// getter
int Socket::getSocketFd(){
    return this->fd_;
}

// setter

void Socket::setFd(int fd) { this->fd_ = fd; }
void Socket::setSocketIndex(int index) { this->socketIndex_ = index; }

// other functions
void Socket::setReqStatus() {
    this->reqStatus.pendingReceive = false;
    this->reqStatus.clen = 0;
    this->reqStatus.readBytes = 0;
}

// private
