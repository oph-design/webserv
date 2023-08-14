#ifndef HEADER_HPP_
#define HEADER_HPP_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "colors.hpp"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

typedef enum e_methodTypes {
  GET = 0,
  POST = 1,
  DELETE = 2,
  OPTIONS = 3,
  HEAD = 4,
  PUT = 5,
  TRACE = 6,
  CONNECT = 7,
  INVALID = 8
} t_methodTypes;

class Request {
 public:
  Request();
  Request(char buffer[BUFFER_SIZE]);
  Request(std::string bufferString);
  ~Request();
  Request(const Request &obj);
  Request &operator=(const Request &obj);

  void addField(std::pair<std::string, std::string> &fieldWithContent);

  const std::string &operator[](const std::string &key) const;
  const std::string &operator[](const char *key) const;
  friend std::ostream &operator<<(std::ostream &stream, const Request &header);

  std::string getPath() const;
  t_methodTypes getRequestMethodType() const;
  std::string getRequestMethodString() const;
  std::string getHTTPVersion() const;
  bool getRequestBodyExists() const;
  std::string getRequestBody() const;

 private:
  void parseRequestLine_(std::string &requestLine);
  void decodeURI_();
  void splitURI_();
  void splitQuery_();

  std::map<std::string, std::string> requestHeaderFields_;
  std::map<std::string, std::string> queryTable_;
  t_methodTypes requestMethodType_;
  std::string queryString_;
  std::string requestMethodString_;
  std::string URI_;
  std::string path_;
  std::string httpVersion_;
  std::string requestBody_;
  bool requestBodyExists_;
};

#endif  // HEADER_HPP_