#include "Location.hpp"

Location::Location() {
  this->autoindex = false;
  this->client_max_body_size = 10000;
  this->index = "index.html";
  this->root = "./";
  this->limit_except.push_back("GET");
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
  return *this;
}
