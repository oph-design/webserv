#include "Location.hpp"

Location::Location() {
  this->autoindex = false;
  this->client_max_body_size = 10000;
  this->index = "index.html";
  this->root = "./";
  this->limit_except.push_back("GET");
    this->duplicates_.clientMaxBodySize = false;
  this->duplicates_.index = false;
  this->duplicates_.root = false;
}

Location::~Location() {}

Location::Location(const Location& obj) { *this = obj; }

Location& Location::operator=(const Location& obj) {
  this->autoindex = obj.autoindex;
  this->client_max_body_size = obj.client_max_body_size;
  this->index = obj.index;
  this->path = obj.path;
  this->root = obj.root;
  this->limit_except = obj.limit_except;
  this->fastcgi_pass = obj.fastcgi_pass;
  this->error_page = obj.error_page;
  this->duplicates_ = obj.duplicates_;

  return *this;
}

std::ostream& operator<<(std::ostream& stream, const Location& location) {
  stream << "\nPath: " << location.path << "\n";
  stream << "autoindex: " << location.autoindex << "\n";
  stream << "client_max_body_size: " << location.client_max_body_size << "\n";
  stream << "index: " << location.index << "\n";
  stream << "root: " << location.root << "\n";
  stream << "limit_except:\n" << location.limit_except;
  stream << "fastcgi_pass:\n" << location.fastcgi_pass;
  stream << "error_page:\n" << location.error_page;
  stream << std::flush;
  return stream;
}

void Location::setDuplicates(const t_duplicates &duplicates)
{
  this->duplicates_ = duplicates;
}

const t_duplicates &Location::getDuplicates() const
{
  return this->duplicates_;
}
