#include "Socket.hpp"

// public
Socket::Socket()
    : fd_(-1),
      socketIndex_(-1),
      socketType_(UNUSED),
      socketOpt_(1),
      listeningSocket_(-1),
      dataSend_(0),
      pendingSend_(false),
      keepAlive_(false),
      timeout_(5) {}

Socket::~Socket() {}

Socket::Socket(const Socket &rhs) { *this = rhs; }

Socket &Socket::operator=(const Socket &rhs) {
  this->fd_ = rhs.fd_;
  this->socketIndex_ = rhs.socketIndex_;
  this->socketType_ = rhs.socketType_;
  this->socketOpt_ = rhs.socketOpt_;
  this->listeningSocket_ = rhs.listeningSocket_;
  this->socketaddr_ = rhs.socketaddr_;
  this->dataSend_ = rhs.dataSend_;
  this->response_ = rhs.response_;
  this->pendingSend_ = rhs.pendingSend_;
  this->keepAlive_ = rhs.keepAlive_;
  this->timestamp_ = rhs.timestamp_;
  this->timeout_ = rhs.timeout_;
  return *this;
}

// getter

const int &Socket::getFd() const { return this->fd_; }
const bool &Socket::getKeepAlive() const { return this->keepAlive_; }

// setter

void Socket::setIdle() {
  fd_ = -1;
  socketIndex_ = -1;
  socketType_ = UNUSED;
  socketOpt_ = 1;
  listeningSocket_ = -1;
  memset(&socketaddr_, 0, sizeof(socketaddr_));
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
  if (this->socketType_ == CLIENT) {
    time_t current = std::time(NULL);
    if ((current - this->timestamp_) >= this->timeout_) return true;
  }
  return false;
}
