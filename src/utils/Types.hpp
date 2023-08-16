#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <iterator>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Config;
class Line;
class Location;

typedef std::vector<Line>::iterator LineIter;

typedef std::vector<Line> LineVector;

typedef std::vector<std::string>::iterator StringIter;

typedef std::vector<Config> ConfigVector;

typedef std::vector<Location> LocationVector;

typedef std::set<std::string> StringSet;

typedef std::pair<int, std::string> ErrorPage;

typedef std::map<int, std::string> ErrorMap;

typedef struct s_duplicates {
  bool index;
  bool root;
  bool clientMaxBodySize;
} Duplicates;

#endif  // TYPES_HPP_
