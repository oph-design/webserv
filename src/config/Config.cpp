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
  std::string buffer;
  int lineCount = 0;
  while (std::getline(file, buffer)) {
    Line line(lineCount, buffer);
    this->content_.push_back(line);
    lineCount++;
    buffer.clear();
  }
  return true;
}

void Config::cleanContent() {
  std::vector<Line> newContent;
  for (std::vector<Line>::iterator iter = this->content_.begin();
       iter != this->content_.end(); ++iter) {
    (*iter).trimWhitespace();
    (*iter).removeComment();
    if (!(*iter).isEmpty()) newContent.push_back(*iter);
  }
  this->content_ = newContent;
}

std::ostream& operator<<(std::ostream& stream, const Config& config) {
  stream << "Line\tError\tContent\n";
  stream << std::boolalpha;
  for (std::vector<Line>::const_iterator iter = config.content_.begin();
       iter != config.content_.end(); ++iter) {
    stream << *iter << "\n";
  }
  stream << std::flush;
  return stream;
}
