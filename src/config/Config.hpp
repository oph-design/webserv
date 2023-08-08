#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifndef STD_CONF_PATH
#define STD_CONF_PATH "./conf/webserv.conf"
#endif  // STD_CONF_PATH

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Line.hpp"

class Config {
 public:
  ~Config();
  Config();
  Config(const Config& obj);
  Config& operator=(const Config& obj);

  bool openFile(int argc, char *argv[]);

 private:
  std::vector<Line> content_;
};

#endif  // CONFIG_HPP_
