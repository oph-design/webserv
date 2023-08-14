#include "Config.hpp"

Config::Config() {
  this->listen = 1234;
  this->client_max_body_size = 10000;
  this->server_name = "localhost";
  this->index = "index.html";
  this->root = "html/";
}

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

std::ostream& operator<<(std::ostream& stream, const Config& config) {
  stream << "listen: " << config.listen << "\n";
  stream << "client_max_body_size: " << config.client_max_body_size << "\n";
  stream << "server_name" << config.server_name << "\n";
  stream << "index: " << config.index << "\n";
  stream << "root: " << config.root << "\n";
  stream << "locations: " << config.locations << "\n";
  stream << "error_page: " << config.error_page << "\n";
  stream << "\n\n";
  stream << std::flush;
  return stream;
}
