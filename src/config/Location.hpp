#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "StreamOperators.hpp"
#include "Types.hpp"

class Location {
  friend class Config;
  friend class ConfigFile;
  friend class ConfigParsing;

 public:
  Location();
  ~Location();
  Location(const Location& obj);
  Location& operator=(const Location& obj);

  friend std::ostream& operator<<(std::ostream& stream,
                                  const Location& location);

  const bool& getAutoindex() const;
  const int& getClientMaxBodySize() const;
  const std::string& getIndex() const;
  const std::string& getPath() const;
  const std::string& getRoot() const;
  const std::string& getUploadPass() const;
  const StringSet& getMethods() const;
  const std::map<std::string, std::string>& getFastcgiPass() const;
  const ErrorMap& getErrorPage() const;

 private:
  bool autoindex_;
  int clientMaxBodySize_;
  std::string index_;
  std::string path_;
  std::string root_;
  std::string uploadPass_;
  StringSet limitExcept_;
  std::map<std::string, std::string> fastcgiPass_;
  ErrorMap errorPage_;

  Duplicates duplicates_;
};

#endif  // LOCATION_HPP_
