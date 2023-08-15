#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "StreamOperators.hpp"

typedef struct s_duplicates {
  bool index;
  bool root;
  bool clientMaxBodySize;
} t_duplicates;

class Location {
 public:
  Location();
  ~Location();
  Location(const Location& obj);
  Location& operator=(const Location& obj);

  friend std::ostream& operator<<(std::ostream& stream,
                                  const Location& location);
  void setDuplicates(const t_duplicates& duplicates);
  const t_duplicates& getDuplicates() const;

  friend class Config;
  friend class ConfigFile;
  friend class ConfigParsing;

 private:
  bool autoindex_;
  int clientMaxBodySize_;
  std::string index_;
  std::string path_;
  std::string root_;
  std::set<std::string> limitExcept;
  std::map<std::string, std::string> fastcgiPass;
  std::map<int, std::string> errorPage;

  t_duplicates duplicates_;
};

#endif  // LOCATION_HPP_
