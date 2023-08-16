#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <iterator>
#include <string>
#include <vector>
#include <map>

class Config;
class Line;
class Location;

typedef std::vector<Line>::iterator LineIter;

typedef std::vector<Line> LineVector;

typedef std::vector<std::string>::iterator StringIter;

typedef std::vector<Config> ConfigVector;

typedef std::vector<Location> LocationVector;

typedef std::set<std::string> StringSet;

typedef std::map<int, std::string> ErrorPage;

#endif  // TYPES_HPP_
