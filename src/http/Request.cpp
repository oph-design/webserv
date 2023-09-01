#include "Request.hpp"

Request::Request() {
  this->requestMethodType_ = INVALID;
  this->requestBodyExists_ = false;
}

Request::Request(char buffer[BUFFER_SIZE]) {
  this->requestBodyExists_ = false;
  std::string string(buffer);
  *this = Request(string);
}

Request::Request(std::string bufferString) {
  this->requestBodyExists_ = false;
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
  this->requestBody_ = bufferString.substr(bufferString.find("\r\n\r\n") + 4,
                                           bufferString.length());
  if (requestBody_.size() != 0) {
    this->requestBodyExists_ = true;
    this->unchunkBody_();
  }
}

void Request::unchunkBody_() {
  try {
    std::string newBody = "";
    size_t clen = atoi((*this)["Content-Length"].c_str());
    if (!((*this)["Transfer-Encoding"] == "chunked")) return;
    while (newBody.length() < clen) {
      int position = requestBody_.find("\r\n");
      int len = std::strtol(requestBody_.substr(0, position).c_str(), NULL, 16);
      requestBody_ = requestBody_.substr(position + 2, requestBody_.length());
      newBody.append(this->requestBody_.substr(0, len));
      requestBody_ = requestBody_.substr(len + 1, requestBody_.length());
    }
    this->requestBody_ = newBody;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

Request::~Request() {}

Request::Request(const Request &obj) { *this = obj; }

Request &Request::operator=(const Request &obj) {
  this->requestHeaderFields_ = obj.requestHeaderFields_;
  this->queryTable_ = obj.queryTable_;
  this->requestMethodType_ = obj.requestMethodType_;
  this->queryString_ = obj.queryString_;
  this->requestMethodString_ = obj.requestMethodString_;
  this->URI_ = obj.URI_;
  this->path_ = obj.path_;
  this->httpVersion_ = obj.httpVersion_;
  this->requestBody_ = obj.requestBody_;
  this->requestBodyExists_ = obj.requestBodyExists_;
  return *this;
}

const std::string &Request::operator[](const std::string &key) const {
  return this->requestHeaderFields_.at(key);
}

const std::string &Request::operator[](const char *key) const {
  std::string stringKey(key);
  return this->requestHeaderFields_.at(stringKey);
}

std::string Request::getPath() const { return this->path_; }

t_methodTypes Request::getRequestMethodType() const {
  return this->requestMethodType_;
}

std::string Request::getQueryString() const { return this->queryString_; }

std::string Request::getRequestBody() const { return this->requestBody_; }

std::string Request::getRequestMethodString() const {
  return this->requestMethodString_;
}

std::string Request::getHTTPVersion() const { return this->httpVersion_; }

bool Request::getRequestBodyExists() const { return this->requestBodyExists_; }

void Request::parseRequestLine_(std::string &requestLine) {
  std::stringstream ss(requestLine);
  std::getline(ss, this->requestMethodString_, ' ');
  std::getline(ss, this->URI_, ' ');
  this->decodeURI_();
  this->splitURI_();
  this->splitQuery_();
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
  else
    this->requestMethodType_ = INVALID;
}

void Request::decodeURI_() {
  std::string newUri;
  for (std::string::iterator iter = this->URI_.begin();
       iter != this->URI_.end(); ++iter) {
    if (*iter == '%' && iter + 1 != this->URI_.end() &&
        iter + 2 != this->URI_.end()) {
      char numberChar[3] = {*(iter + 1), *(iter + 2), 0};
      char *endptr = NULL;
      u_int64_t numberInt = std::strtoul(numberChar, &endptr, 16);
      if (endptr && *endptr == 0) {
        newUri += static_cast<char>(numberInt);
        iter += 2;
      }
    } else {
      newUri += *iter;
    }
  }
  this->URI_ = newUri;
}

void Request::splitURI_() {
  std::size_t queryBegin = this->URI_.find('?');
  if (queryBegin != this->URI_.npos && queryBegin + 1 != this->URI_.npos) {
    this->path_ = this->URI_.substr(0, queryBegin);
    this->queryString_ = this->URI_.substr(queryBegin + 1);
  } else {
    this->path_ = this->URI_;
  }
}

void Request::splitQuery_() {
  std::stringstream ss(this->queryString_);
  std::string queryLine;
  while (std::getline(ss, queryLine, '&')) {
    if (queryLine.size() == 0) continue;
    std::size_t equalPosition = queryLine.find('=');
    if (equalPosition != queryLine.npos && equalPosition + 1 != queryLine.npos)
      this->queryTable_.insert(
          std::make_pair(queryLine.substr(0, equalPosition),
                         queryLine.substr(equalPosition + 1)));
    else
      this->queryTable_.insert(std::make_pair(queryLine, ""));
    queryLine.clear();
  }
}

std::ostream &operator<<(std::ostream &stream, const Request &header) {
  stream << "Method=" << header.requestMethodString_ << "\n";
  stream << "URI_=" << header.URI_ << "\n";
  stream << "httpVersion=" << header.httpVersion_ << "\n";
  stream << "PATH=" << header.path_ << "\n";

  for (std::map<std::string, std::string>::const_iterator iter =
           header.requestHeaderFields_.begin();
       iter != header.requestHeaderFields_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    stream << "\n";
  }
  if (header.requestBodyExists_) stream << header.requestBody_;
  stream << "QUERY=" << header.queryString_ << "\n";
  for (std::map<std::string, std::string>::const_iterator iter =
           header.queryTable_.begin();
       iter != header.queryTable_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    stream << "\n";
  }
  return stream;
}
