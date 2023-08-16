#include "CgiConnector.hpp"

CgiConnector::InOutHandler::InOutHandler() {
  pipe(this->pipes_);
  this->stdIn_ = dup(0);
  this->stdOut_ = dup(1);
}

CgiConnector::InOutHandler::~InOutHandler() {
  dup2(this->stdIn_, 0);
  dup2(this->stdOut_, 1);
  close(this->stdIn_);
  close(this->stdOut_);
}

void CgiConnector::InOutHandler::dupInChild() {
  dup2(this->pipes_[1], 1);
  close(this->pipes_[1]);
  close(this->pipes_[0]);
}

void CgiConnector::InOutHandler::dupInParent() {
  dup2(this->pipes_[0], 0);
  close(this->pipes_[1]);
  close(this->pipes_[0]);
}
