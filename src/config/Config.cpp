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

std::vector<Config>& Config::handleDuplicates(std::vector<Config>& configs) {
  std::vector<Config> newVector;
  for (std::vector<Config>::iterator iterOuter = configs.begin();
       iterOuter != configs.end(); ++iterOuter) {
    bool duplicate = false;
    for (std::vector<Config>::iterator iterInner = iterOuter;
         iterInner != configs.end(); ++iterInner) {
      if (iterInner == iterOuter) continue;
      if (iterInner->listen == iterOuter->listen &&
          iterInner->server_name == iterOuter->server_name)
        duplicate = true;
    }
    iterOuter->fillLocations_();
    if (duplicate == false) newVector.push_back(*iterOuter);
  }
  configs = newVector;
  return configs;
}

void Config::fillLocations_() {
  for (std::vector<Location>::iterator iter = this->locations.begin();
       iter != this->locations.end(); ++iter) {
    if (iter->getDuplicates().clientMaxBodySize == false)
      iter->client_max_body_size = this->client_max_body_size;
    if (iter->getDuplicates().index == false) iter->index = this->index;
    if (iter->getDuplicates().root == false) iter->root = this->root;
    std::map<int, std::string> newErrorPage = this->error_page;
    newErrorPage.insert(iter->error_page.begin(), iter->error_page.end());
    iter->error_page = newErrorPage;
  }
}

std::ostream& operator<<(std::ostream& stfream, const Config& config) {
  stream << "listen: " << config.listen << "\n";
  stream << "client_max_body_size: " << config.client_max_body_size << "\n";
  stream << "server_name: " << config.server_name << "\n";
  stream << "index: " << config.index << "\n";
  stream << "root: " << config.root << "\n";
  stream << "locations: " << config.locations << "\n";
  stream << "error_page: " << config.error_page << "\n";
  stream << "\n\n";
  stream << std::flush;
  return stream;
}
