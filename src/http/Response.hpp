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
#include "PrintVerbose.hpp"
#include "Request.hpp"
#include "Status.hpp"
#include "Utils.hpp"
#include "colors.hpp"

typedef std::pair<std::string, std::string> contentField;
typedef std::map<std::string, std::string> contentMap;

class Response {
public:
  Response(Request &request, const Location &location);
  Response(const Response &rhs);
  Response &operator=(const Response &rhs);
  ~Response();

  const std::string &operator[](const std::string &key);
  const std::string &operator[](const char *key);
  friend std::ostream &operator<<(std::ostream &stream, const Response &resp);
  const std::string &getBody() const;
  std::string getHeader() const;
  static contentMap createTypeMap();

private:
  void readBody_(const std::string &dir);
  void findType_(const std::string &url);
  void handleGetRequest_(Request &request, const std::string &uri);
  void handlePostRequest_(const Request &request, const std::string &uri);
  void handleDeleteRequest_(const Request &request, const std::string &uri);
  void createFile_(const std::string &filename, const std::string &ext,
                   const std::string &data);
  void buildJsonBody_();
  void serveCgi_(const Request &request);
  bool prerequisites_(const std::string &method, const Request &request);
  bool redirect();

  static bool isForbiddenPath_(const std::string &dir);
  static bool isFolder_(const std::string &uri);
  void serveFolder_(const std::string &path);
  std::string createFolderBody_(const std::string &path);
  static std::deque<std::string> getFilesInFolder_(const std::string &path);

  static contentMap fileTypes_;
  contentMap header_;
  Status status_;
  std::string body_;
  const Location &location_;
  std::string type_;
};

#endif // !RESPONSE_HPP
