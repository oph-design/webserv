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
#include <stdexcept>
#include <string>

#include "Request.hpp"
#include "Status.hpp"
#include "ToString.hpp"

#define CHUNKSIZE 1024

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string> contentMap;

class Response {
 public:
  Response();
  Response(const Request& request);
  Response(const Response& rhs);
  Response& operator=(const Response& rhs);
  ~Response();

  const std::string& operator[](const std::string& key);
  const std::string& operator[](const char* key);
  friend std::ostream& operator<<(std::ostream& stream, const Response& resp);
  const std::list<std::string> getBodyChunked() const;
  const std::string& getBody() const;
  const std::string getHeader() const;
  static contentMap createTypeMap();

 private:
  static std::string buildChunk_(std::string line);
  std::string readBody_(std::string dir);
  std::string findType_(std::string url);
  void handleGetRequest(const Request& request);
  void handlePostRequest(const Request& request);
  void handleDeleteRequest(const Request& request);

  static contentMap fileTypes_;
  contentMap header_;
  Status status_;
  std::string body_;
};

#endif  // !RESPONSE_HPP