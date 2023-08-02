#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <algorithm>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Response {
 public:
  Response();
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();

 private:
  std::string body_;
  std::map<std::string, std::string> header_;
};

#endif  // !RESPONSE_HPP
