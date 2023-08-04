#include "Header.hpp"

Header::Header() {
  this->method_ = INVALID;
}

Header::Header(char buffer[BUFFER_SIZE]) {
  std::string string(buffer);
  *this = Header(string);
}

Header::Header(std::string bufferString) {
  std::stringstream ss(bufferString);
  std::string line;
  std::getline(ss, line);
  line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
  line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
  this->requestLine_ = line;
  this->parseRequestLine_(line);
  while (true) {
    line.clear();
    std::getline(ss, line);
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    if (line.size() == 0) break;
    std::pair<std::string, std::string> keyPair;
    keyPair.first = line.substr(0, line.find(": "));
    keyPair.second =
        line.substr(line.find(": ") + 2, line.end() - line.begin());
    this->headerContent_.insert(keyPair);
  }
}

Header::~Header() {}

Header::Header(const Header &obj) { *this = obj; }

Header &Header::operator=(const Header &obj) {
  this->requestLine_ = obj.requestLine_;
  this->headerContent_ = obj.headerContent_;
  this->method_ = obj.method_;
  this->methodString_ = obj.methodString_;
  this->URI_ = obj.URI_;
  this->httpVersion_ = obj.httpVersion_;
  return *this;
}

const std::string &Header::operator[](const std::string &key) const {
  return this->headerContent_.at(key);
}

const std::string &Header::operator[](const char *key) const {
  std::string stringKey(key);
  return this->headerContent_.at(stringKey);
}

void Header::parseRequestLine_(std::string &requestLine)
{
  std::stringstream ss(requestLine);
  std::getline(ss, this->methodString_, ' ');
  std::getline(ss, this->URI_, ' ');
  std::getline(ss, this->httpVersion_, ' ');

  if (this->methodString_ == "GET")
    this->method_ = GET;
  else if (this->methodString_ == "POST")
    this->method_ = POST;
  else if (this->methodString_ == "DELETE")
    this->method_ = DELETE;
  else if (this->methodString_ == "OPTIONS")
    this->method_ = OPTIONS;
  else if (this->methodString_ == "HEAD")
    this->method_ = HEAD;
  else if (this->methodString_ == "PUT")
    this->method_ = PUT;
  else if (this->methodString_ == "TRACE")
    this->method_ = TRACE;
  else if (this->methodString_ == "CONNECT")
    this->method_ = CONNECT;
  else if (this->methodString_ == "INVALID")
    this->method_ = INVALID;
}

std::ostream &operator<<(std::ostream &stream, const Header &header) {
  stream << header.requestLine_ << "\n";
  stream << "Method String: " << header.methodString_ << "\n";
  stream << "URI_: " << header.URI_ << "\n";
  stream << "httpVersion: " << header.httpVersion_ << "\n";
  for (std::map<std::string, std::string>::const_iterator iter =
           header.headerContent_.begin();
       iter != header.headerContent_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    if (std::next(iter) != header.headerContent_.end()) stream << "\n";
  }
  return stream;
}
