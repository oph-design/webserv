#include "Header.hpp"

Header::Header() {
  this->headerContent_.insert(
      std::make_pair(std::string("key1"), std::string("content1")));
  this->headerContent_.insert(
      std::make_pair(std::string("key2"), std::string("content2")));
  this->headerContent_.insert(
      std::make_pair(std::string("key3"), std::string("content3")));
  this->headerContent_.insert(
      std::make_pair(std::string("key4"), std::string("content4")));
}

Header::Header(char buffer[BUFFER_SIZE]) {
  std::string string(buffer);
  Header header(string);
}

Header::Header(std::string &bufferString) { (void)bufferString; }

Header::~Header() {}

Header::Header(const Header &obj) { *this = obj; }

Header &Header::operator=(const Header &obj) {
  this->headerContent_ = obj.headerContent_;
  return *this;
}

std::ostream &operator<<(std::ostream &stream, const Header &header) {
  for (std::map<std::string, std::string>::const_iterator iter =
           header.headerContent_.begin();
       iter != header.headerContent_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    if (std::next(iter) != header.headerContent_.end()) stream << ", ";
  }
  return stream;
}
