#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string>::iterator mapIterator;

class Response {
 public:
  Response();
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();
  std::string toString();

 private:
  std::string body_;
  std::map<std::string, std::string> header_;
};

#endif  // !RESPONSE_HPP
