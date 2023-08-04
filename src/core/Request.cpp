#include "Request.hpp"

Request::Request() { this->requestMethodType_ = INVALID; }

Request::Request(char buffer[BUFFER_SIZE]) {
  std::string string(buffer);
  *this = Request(string);
}

Request::Request(std::string bufferString) {
  std::stringstream ss(bufferString);
  std::string line;
  std::getline(ss, line);
  line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
  line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
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
    this->requestHeaderFields_.insert(keyPair);
  }
}

Request::~Request() {}

Request::Request(const Request &obj) { *this = obj; }

Request &Request::operator=(const Request &obj) {
  this->requestHeaderFields_ = obj.requestHeaderFields_;
  this->requestMethodType_ = obj.requestMethodType_;
  this->requestMethodString_ = obj.requestMethodString_;
  this->URI_ = obj.URI_;
  this->httpVersion_ = obj.httpVersion_;
  return *this;
}

const std::string &Request::operator[](const std::string &key) const {
  return this->requestHeaderFields_.at(key);
}

const std::string &Request::operator[](const char *key) const {
  std::string stringKey(key);
  return this->requestHeaderFields_.at(stringKey);
}

void Request::parseRequestLine_(std::string &requestLine) {
  std::stringstream ss(requestLine);
  std::getline(ss, this->requestMethodString_, ' ');
  std::getline(ss, this->URI_, ' ');
  std::getline(ss, this->httpVersion_, ' ');

  if (this->requestMethodString_ == "GET")
    this->requestMethodType_ = GET;
  else if (this->requestMethodString_ == "POST")
    this->requestMethodType_ = POST;
  else if (this->requestMethodString_ == "DELETE")
    this->requestMethodType_ = DELETE;
  else if (this->requestMethodString_ == "OPTIONS")
    this->requestMethodType_ = OPTIONS;
  else if (this->requestMethodString_ == "HEAD")
    this->requestMethodType_ = HEAD;
  else if (this->requestMethodString_ == "PUT")
    this->requestMethodType_ = PUT;
  else if (this->requestMethodString_ == "TRACE")
    this->requestMethodType_ = TRACE;
  else if (this->requestMethodString_ == "CONNECT")
    this->requestMethodType_ = CONNECT;
  else if (this->requestMethodString_ == "INVALID")
    this->requestMethodType_ = INVALID;
}

std::ostream &operator<<(std::ostream &stream, const Request &header) {
  stream << "Method=" << header.requestMethodString_ << "\n";
  stream << "URI_=" << header.URI_ << "\n";
  stream << "httpVersion=" << header.httpVersion_ << "\n";
  for (std::map<std::string, std::string>::const_iterator iter =
           header.requestHeaderFields_.begin();
       iter != header.requestHeaderFields_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    if (std::next(iter) != header.requestHeaderFields_.end()) stream << "\n";
  }
  return stream;
}
