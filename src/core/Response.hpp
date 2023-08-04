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
#include <vector>
#include <iterator>
#include <sstream>
#include <string>

typedef std::pair<std::string, std::string> contentField;
typedef std::vector<contentField> contentVector;

class Response {
 public:
  Response();
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();
  std::string toString() const;

 private:
  static std::string readBody_(std::string dir);
  std::string body_;
  contentVector header_;
};

#endif  // !RESPONSE_HPP
