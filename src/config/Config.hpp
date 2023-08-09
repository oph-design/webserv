#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifndef STD_CONF_PATH
#define STD_CONF_PATH "./conf/webserv.conf"
#endif  // STD_CONF_PATH

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Line.hpp"

class Config {
 public:
  ~Config();
  Config();
  Config(const Config& obj);
  Config& operator=(const Config& obj);

  bool openFile(int argc, char* argv[]);
  void cleanContent();

  friend std::ostream &operator<<(std::ostream &stream, const Config &config);
 private:
  std::vector<Line> content_;
};

#endif  // CONFIG_HPP_
