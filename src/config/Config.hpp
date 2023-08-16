#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifndef STANDARD_PORT
#define STANDARD_PORT 1234
#endif

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Location.hpp"
#include "ToString.hpp"
#include "Types.hpp"

class Config {
  friend class ConfigFile;
  friend class Location;
  friend class ConfigParsing;

 public:
  Config();
  ~Config();
  Config(const Config& obj);
  Config& operator=(const Config& obj);

  friend std::ostream& operator<<(std::ostream& stream, const Config& config);

 private:
  static ConfigVector& handleDuplicates_(ConfigVector& configs);
  void fillLocations_();
  static void fillAllLocations_(ConfigVector& configs);

  static void fillHostPort_(ConfigVector& configs);
  static void makeHostPortUnique_(ConfigVector& configs);
  static void getPortsFromHostPort_(ConfigVector& configs);
  static void deleteEmptyServer_(ConfigVector& configs);

  std::set<int> listen_;
  int clientMaxBodySize_;
  std::string serverName_;
  std::string index_;
  std::string root_;
  LocationVector locations_;
  ErrorMap errorPage_;

  StringSet hostPort_;
};

#endif  // CONFIG_HPP_
