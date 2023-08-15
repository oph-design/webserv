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

class Config {
 public:
  Config();
  ~Config();
  Config(const Config& obj);
  Config& operator=(const Config& obj);

  friend std::ostream& operator<<(std::ostream& stream, const Config& config);

  static std::vector<Config>& handleDuplicates(std::vector<Config>& configs);

  friend class ConfigFile;
  friend class Location;
  friend class ConfigParsing;

 private:
  void fillLocations_();
  static void fillAllLocations(std::vector<Config>& configs);

  static void fillHostPort_(std::vector<Config>& configs);
  static void makeHostPortUnique_(std::vector<Config>& configs);
  static void getPortsFromHostPort_(std::vector<Config>& configs);
  static void deleteEmptyServer_(std::vector<Config>& configs);

  std::set<int> listen_;
  int clientMaxBodySize_;
  std::string serverName_;
  std::string index_;
  std::string root_;
  std::vector<Location> locations_;
  std::map<int, std::string> errorPage_;

  std::set<std::string> hostPort_;
};

#endif  // CONFIG_HPP_
