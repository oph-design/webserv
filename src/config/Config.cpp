#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->content_ = obj.content_;
  return *this;
}

bool Config::openFile(int argc, char* argv[]) {
  std::string path;
  if (argc < 2) {
    path = STD_CONF_PATH;
  } else if (argc == 2) {
    path = argv[1];
  } else {
    std::cerr << "Error: Webserv requires zero to one arguments" << std::endl;
    return false;
  }
  std::ifstream file;
  file.open(path.c_str(), std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error: Can't open config file" << std::endl;
    return false;
  }
  return true;
}
