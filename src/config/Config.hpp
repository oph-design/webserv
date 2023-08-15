#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <map>
#include <string>
#include <vector>

#include "Location.hpp"

class Config {
 public:
  Config();
  ~Config();
  Config(const Config& obj);
  Config& operator=(const Config& obj);

  int listen;
  int client_max_body_size;
  std::string server_name;
  std::string index;
  std::string root;
  std::vector<Location> locations;
  std::map<int, std::string> error_page;

  friend std::ostream& operator<<(std::ostream& stream, const Config& config);

 private:
};

#endif  // CONFIG_HPP_
