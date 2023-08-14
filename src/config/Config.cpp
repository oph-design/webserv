#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->listen = obj.listen;
  this->client_max_body_size = obj.client_max_body_size;
  this->server_name = obj.server_name;
  this->index = obj.index;
  this->root = obj.root;
  this->locations = obj.locations;
  this->error_page = obj.error_page;
  return *this;
}

  int listen;
  int client_max_body_size;
  std::string server_name;
  std::string index;
  std::string root;
  std::vector<Location> locations;
  std::map<int, std::string> error_page;