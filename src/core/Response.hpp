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

  std::list<std::string> getBodyChunked() const;
  std::string getBody() const;
  std::string getHeader() const;
  static void fillFileTypes();

 private:
  static std::string buildChunk(std::string line);
  std::string readBody_(std::string dir);
  std::string findType_(std::string url);

  static typeMap fileTypes_;
  contentVector header_;
  std::string status_;
  std::string body_;
};

#endif  // !RESPONSE_HPP
