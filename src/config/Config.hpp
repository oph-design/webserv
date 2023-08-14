#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <vector>
#include <string>
#include <map>

#include "Location.hpp"

class Config {
 public:
  Config();
  ~Config();
  Config(const Config & obj);
  Config& operator=(const Config& obj);

  std::vector<Location> locations;
  std::map<std::string, std::string> params;
 private:
};

#endif  // CONFIG_HPP_
