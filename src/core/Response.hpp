#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Request.hpp"

#define CHUNKSIZE 1024

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string> typeMap;
typedef std::vector<contentField> contentVector;

class Response {
 public:
  Response();
  Response(Request request);
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();

  std::list<std::string> getBody() const;
  std::string getFullBody() const;
  std::string getHeader() const;
  static void fillFileTypes();

 private:
  std::string readBody_(std::string dir);
  void chunkBody_(std::string newBody);
  std::string findType_(std::string url);

  static typeMap fileTypes_;
  contentVector header_;
  std::list<std::string> body_;
  std::string status_;
  std::string bodyFull_;
};

#endif  // !RESPONSE_HPP
