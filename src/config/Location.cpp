#include "Location.hpp"

Location::Location() {
  this->autoindex_ = false;
  this->clientMaxBodySize_ = 10000;
  this->index_ = "index.html";
  this->root_ = "./";
  this->limitExcept.insert("GET");
  this->duplicates_.clientMaxBodySize = false;
  this->duplicates_.index = false;
  this->duplicates_.root = false;
}

Location::~Location() {}

Location::Location(const Location& obj) { *this = obj; }

Location& Location::operator=(const Location& obj) {
  this->autoindex_ = obj.autoindex_;
  this->clientMaxBodySize_ = obj.clientMaxBodySize_;
  this->index_ = obj.index_;
  this->path_ = obj.path_;
  this->root_ = obj.root_;
  this->limitExcept = obj.limitExcept;
  this->fastcgiPass = obj.fastcgiPass;
  this->errorPage = obj.errorPage;
  this->duplicates_ = obj.duplicates_;
  return *this;
}

std::ostream& operator<<(std::ostream& stream, const Location& location) {
  stream << "\nPath: " << location.path_ << "\n";
  stream << "autoindex: " << location.autoindex_ << "\n";
  stream << "client_max_body_size: " << location.clientMaxBodySize_ << "\n";
  stream << "index: " << location.index_ << "\n";
  stream << "root: " << location.root_ << "\n";
  stream << "limit_except:\n" << location.limitExcept;
  stream << "fastcgi_pass:\n" << location.fastcgiPass;
  stream << "error_page:\n" << location.errorPage;
  stream << std::flush;
  return stream;
}

void Location::setDuplicates(const Duplicates& duplicates) {
  this->duplicates_ = duplicates;
}
