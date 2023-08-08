#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->content_ = obj.content_;
  return *this;
}

int Config::openConfigFile(int argc, char* argv[]) {
  std::string path;
  if (argc < 2)
    path = STD_CONF_PATH;
  else
    path = argv[1];
  std::cout << path << std::endl;
  return 0;
}
