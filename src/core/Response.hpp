#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string> typeMap;
typedef std::vector<contentField> contentVector;

class Response {
 public:
  Response();
  Response(std::string url);
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();

  std::string toString() const;

 private:
  static std::string readBody_(std::string dir);
  static std::string findType(std::string url);
  void fillFileTypes_();

  typeMap fileTypes_;
  contentVector header_;
  std::string body_;
};

#endif  // !RESPONSE_HPP
