#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Utils.hpp"

typedef std::set<std::string> StringSet;
typedef std::map<int, std::string> ErrorMap;

typedef struct s_duplicates {
  bool index;
  bool root;
  bool upload_pass;
  bool cgi_pass;
  bool clientMaxBodySize;
} Duplicates;

class Location {
  friend class Config;
  friend class ConfigFile;
  friend class ConfigParsing;

 public:
  Location();
  ~Location();
  Location(const Location &obj);
  Location &operator=(const Location &obj);

  friend std::ostream &operator<<(std::ostream &stream,
                                  const Location &location);

  const std::string &getRewrite() const;
  const bool &getAutoindex() const;
  const int &getClientMaxBodySize() const;
  const std::string &getIndex() const;
  const std::string &getPath() const;
  const std::string &getRoot() const;
  const std::string &getUploadPass() const;
  const StringSet &getMethods() const;
  const std::string &getCgiPass() const;
  bool getCgiPr() const;
  const ErrorMap &getErrorPage() const;
  bool methodAllowed(const std::string &method) const;
  bool maxBodyReached(const std::size_t &contentLength) const;
  bool operator>(const Location &rhs) const;

 private:
  bool autoindex_;
  bool cgiProcessing_;
  int clientMaxBodySize_;
  std::string index_;
  std::string path_;
  std::string root_;
  std::string cgiPass_;
  std::string uploadPass_;
  StringSet limitExcept_;
  ErrorMap errorPage_;
  std::string rewrite_;

  Duplicates duplicates_;
};

#endif  // LOCATION_HPP_
