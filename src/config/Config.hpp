#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <map>
#include <string>
#include <vector>

#include "Location.hpp"

typedef struct s_duplicates {
  bool index;
  bool root;
  bool clientMaxBodySize;
} t_duplicates;

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

  static std::vector<Config>& handleDuplicates(std::vector<Config>& configs);

 private:
  void fillLocations_();
  t_duplicates duplicates_;
};

#endif  // CONFIG_HPP_
