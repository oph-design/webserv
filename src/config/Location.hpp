#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Types.hpp"
#include "StreamOperators.hpp"



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

 private:
  bool autoindex_;
  int clientMaxBodySize_;
  std::string index_;
  std::string path_;
  std::string root_;
  StringSet limitExcept;
  std::map<std::string, std::string> fastcgiPass;
  ErrorMap errorPage;

  Duplicates duplicates_;
};

#endif  // LOCATION_HPP_
