#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "CgiConnector.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Status.hpp"
#include "Utils.hpp"

#define VERBOSE false

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string> contentMap;

class Response {
 public:
  Response(Request &request, Config &config, const Location &location);
  Response(const Response &rhs);
  Response &operator=(const Response &rhs);
  ~Response();

  const std::string &operator[](const std::string &key);
  const std::string &operator[](const char *key);
  friend std::ostream &operator<<(std::ostream &stream, const Response &resp);
  const std::string &getBody() const;
  const std::string getHeader() const;
  static contentMap createTypeMap();

 private:
  void readBody_(std::string dir);
  void findType_(std::string url);
  void handleGetRequest_(Request &request, std::string uri);
  void handlePostRequest_(const Request &request, std::string uri);
  void handleDeleteRequest_(const Request &request, std::string uri);
  void createFile_(std::string filename, std::string ext, std::string data);
  void buildJsonBody_();
  void serveCgi_(const Request &request);
  void prerequisits_(std::string meth, const Request &request);

  static bool isForbiddenPath_(const std::string &dir);
  static bool isFolder_(std::string uri);
  void serveFolder_(std::string path);
  std::string createFolderBody_(std::string path);
  std::deque<std::string> getFilesInFolder_(std::string path);

  static contentMap fileTypes_;
  contentMap header_;
  Status status_;
  std::string body_;
  Config &config_;
  const Location &location_;
  std::string type_;
};

#endif  // !RESPONSE_HPP
