#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

Location::Location(const Location& obj) { *this = obj; }

Location& Location::operator=(const Location& obj) {
  this->params = obj.params;
  this->path = obj.path;
  return *this;
}
