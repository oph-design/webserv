#include "Response.hpp"

Response::Response() : body_("Server is online") {}

Response::Response(const Response& rhs) { *this = rhs; }

Response& Response::operator=(const Response& rhs) {
  this->body_ = rhs.body_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->body_.clear(); }

