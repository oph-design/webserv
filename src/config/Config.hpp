#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifndef FALLBACK_VALUES
#define FALLBACK_VALUES
#define FALLBACK_PORT 1234
#define FALLBACK_SERVERNAME "localhost"
#define FALLBACK_INDEX "index.html"
#define FALLBACK_ROOT "html/"
#define FALLBACK_CLIENTMAXBODYSIZE 10000
#endif  // FALLBACK_VALUES

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

  const int& getClientMaxBodySize() const;
  const std::string& getServerName() const;
  const std::string& getIndex() const;
  const std::string& getRoot() const;
  const LocationVector& getLocations() const;
  Location getLocationByPath(std::string index) const;
  const ErrorMap& getErrorPage() const;
  const int& getPort() const;

 private:
  static ConfigVector& handleDuplicates_(ConfigVector& configs);
  void fillLocations_();
  static void fillAllLocations_(ConfigVector& configs);

  static void fillHostPort_(ConfigVector& configs);
  static void makeHostPortUnique_(ConfigVector& configs);
  static void getPortsFromHostPort_(ConfigVector& configs);
  static void deleteEmptyServer_(ConfigVector& configs);

  std::set<int> listen_;
  int port_;
  int clientMaxBodySize_;
  std::string serverName_;
  std::string index_;
  std::string root_;
  LocationVector locations_;
  ErrorMap errorPage_;

  StringSet hostPort_;
};

#endif  // CONFIG_HPP_
