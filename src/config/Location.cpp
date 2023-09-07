#include "Location.hpp"

#include "Types.hpp"

Location::Location() {
  this->autoindex_ = false;
  this->cgiProcessing_ = false;
  this->clientMaxBodySize_ = 10000;
  this->index_ = "index.html";
  this->root_ = "./";
  this->limitExcept_.insert("GET");
  this->duplicates_.clientMaxBodySize = false;
  this->duplicates_.index = false;
  this->duplicates_.root = false;
  this->duplicates_.upload_pass = false;
  this->duplicates_.cgi_pass = false;
}

Location::~Location() {}

Location::Location(const Location& obj) { *this = obj; }

Location& Location::operator=(const Location& obj) {
  this->autoindex_ = obj.autoindex_;
  this->cgiProcessing_ = obj.cgiProcessing_;
  this->clientMaxBodySize_ = obj.clientMaxBodySize_;
  this->index_ = obj.index_;
  this->path_ = obj.path_;
  this->root_ = obj.root_;
  this->uploadPass_ = obj.uploadPass_;
  this->limitExcept_ = obj.limitExcept_;
  this->cgiPass_ = obj.cgiPass_;
  this->errorPage_ = obj.errorPage_;
  this->rewrite_ = obj.rewrite_;
  this->duplicates_ = obj.duplicates_;
  return *this;
}

bool Location::methodAllowed(const std::string& method) const {
  StringSet::const_iterator it = this->limitExcept_.find(method);
  if (it != this->limitExcept_.end()) return true;
  return false;
}

bool Location::maxBodyReached(const std::size_t& contentLength) const {
  return (static_cast<size_t>(this->clientMaxBodySize_) < contentLength);
}

std::ostream& operator<<(std::ostream& stream, const Location& location) {
  stream << "\nPath: " << location.path_ << "\n";
  stream << "autoindex: " << location.autoindex_ << "\n";
  stream << "client_max_body_size: " << location.clientMaxBodySize_ << "\n";
  stream << "index: " << location.index_ << "\n";
  stream << "root: " << location.root_ << "\n";
  stream << "upload_pass: " << location.uploadPass_ << "\n";
  stream << "rewrite: " << location.rewrite_ << "\n";
  stream << "limit_except:\n" << location.limitExcept_;
  stream << "fastcgi_pass:\n" << location.cgiPass_;
  stream << "error_page:\n" << location.errorPage_;
  stream << std::flush;
  return stream;
}
