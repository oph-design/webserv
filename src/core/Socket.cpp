#include "Socket.hpp"

// public
Socket::Socket()
    : fd_(-1),
      socketIndex_(-1),
      inUse_(false),
      socketType_(UNUSED),
      socketOpt_(1),
      listeningSocket_(-1),
      dataSend_(0),
      pendingSend_(false),
      keepAlive_(true),
      timeout_(5) {}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
  this->fd_ = rhs.fd_;
  this->socketIndex_ = rhs.socketIndex_;
  this->inUse_ = rhs.inUse_;
  this->socketType_ = rhs.socketType_;
  this->boundServerAdress_ = rhs.boundServerAdress_;
  this->socketOpt_ = rhs.socketOpt_;
  this->listeningSocket_ = rhs.listeningSocket_;
  this->servaddr_ = rhs.servaddr_;
  this->dataSend_ = rhs.dataSend_;
  this->response_ = rhs.response_;
  this->pendingSend_ = rhs.pendingSend_;
  this->keepAlive_ = rhs.keepAlive_;
  return *this;
}

// getter
int Socket::getFd() { return this->fd_; }
bool Socket::getKeepAlive() const { return this->keepAlive_; }
// setter

void Socket::setIdle() {
  fd_ = -1;
  socketIndex_ = -1;
  inUse_ = false;
  socketType_ = UNUSED;
  memset(&boundServerAdress_, 0, sizeof(boundServerAdress_));
  socketOpt_ = 1;
  listeningSocket_ = -1;
  memset(&servaddr_, 0, sizeof(servaddr_));
  dataSend_ = 0;
  response_.clear();
  pendingSend_ = false;
  keepAlive_ = false;
}

// other functions
void Socket::setReqStatus() {
  this->reqStatus.pendingReceive = false;
  this->reqStatus.chunked = false;
  this->reqStatus.clen = 0;
  this->reqStatus.readBytes = 0;
}

void Socket::setTimestamp() { this->timestamp_ = std::time(NULL); }

bool Socket::checkTimeout() {
  if (this->inUse_) {
    time_t current = std::time(NULL);
    if (difftime(current, this->timestamp_) >= this->timeout_) return true;
  }
  return false;
}
