#ifndef RESPONSE_HPP
#define RESPONSE_HPP

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
  std::string readBody_(std::string dir);
  std::string findType_(std::string url);
  void handleGetRequest_(Request &request);
  void handlePostRequest_(const Request &request);
  void handleDeleteRequest_(const Request &request);
  void createFile_(std::string filename, std::string ext, std::string data,
                   std::string path);
  void buildJsonBody_();
  void serveCgi_(const Request &request);
  void parseConfig(std::string path);

  static bool isFolder_(std::string uri);
  void serveFolder_(Request &request);
  static std::string createFolderBody_(const Request &request);
  static std::deque<std::string> getFilesInFolder_(
      const std::string &root, const std::string &folderPath);

  static contentMap fileTypes_;
  contentMap header_;
  Status status_;
  std::string body_;
  Config &config_;
  const Location &location_;
};

#endif  // !RESPONSE_HPP
