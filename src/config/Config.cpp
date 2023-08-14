#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->locations = obj.locations;
  this->params = obj.params;
  return *this;
}
