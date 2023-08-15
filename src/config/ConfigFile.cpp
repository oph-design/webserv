#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::~ConfigFile() {}

ConfigFile::ConfigFile(const ConfigFile& obj) { *this = obj; }

ConfigFile& ConfigFile::operator=(const ConfigFile& obj) {
  this->content_ = obj.content_;
  return *this;
}

bool ConfigFile::openFile(int argc, char* argv[]) {
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

void ConfigFile::cleanContent() {
  std::vector<Line> newContent;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    iter->trimWhitespace();
    iter->removeComment();
    if (!iter->isEmpty()) newContent.push_back(*iter);
  }
  this->content_ = newContent;
}

void ConfigFile::vaildateConfigFile() {
  this->checkSeparator();
  this->checkConfigBlocks();
}

bool ConfigFile::isValid() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (!iter->isValid()) return false;
  }
  return true;
}

void ConfigFile::checkSeparator() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->last() != ';' && iter->last() != '{' && iter->last() != '}')
      iter->addError("missing Separator");
  }
}

void ConfigFile::checkConfigBlocks() {
  std::list<LineIter> openings;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->last() == '{') {
      openings.push_back(iter);
    } else if (iter->last() == '}') {
      if (openings.size() == 0)
        iter->addError("Unexpected block closing");
      else
        openings.pop_back();
    }
  }
  if (openings.size() != 0) {
    for (std::list<LineIter>::iterator iter = openings.begin();
         iter != openings.end(); ++iter) {
      (*iter)->addError("Missing block closing");
    }
  }
}

void ConfigFile::removeSemiColon() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->last() == ';') iter->removeSemiColon();
  }
}

std::vector<Config> ConfigFile::createConfig() {
  std::vector<Config> configVector;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->getLine() == "server {") {
      configVector.push_back(parseServer_(iter));
    }
  }
  return configVector;
}

Config ConfigFile::parseServer_(LineIter& iter) {
  Config config;
  ++iter;
  for (; iter != this->content_.end(); ++iter) {
    if (iter->firstWord() == "location" && iter->last() == '{')
      config.locations.push_back(parseLocation_(iter));
    else if (iter->firstWord() == "listen")
      config.listen = parseListen(*iter);
    else if (iter->firstWord() == "client_max_body_size")
      config.client_max_body_size = parseCientMaxBodySize(*iter);
    else if (iter->firstWord() == "server_name")
      config.server_name = parseServerName(*iter);
    else if (iter->firstWord() == "index")
      config.index = parseIndex(*iter);
    else if (iter->firstWord() == "root")
      config.root = parseRoot(*iter);
    else if (iter->firstWord() == "error_page")
      config.error_page.insert(parseErrorPage(*iter));
    else if (iter->getLine() == "}")
      break;
    else
      iter->addError("unknown option" + iter->firstWord());
  }
  return config;
}

Location ConfigFile::parseLocation_(LineIter& iter) {
  Location location;
  location.path = parsePath(*iter);
  ++iter;
  for (; iter != this->content_.end(); ++iter) {
    if (iter->firstWord() == "limit_except" && iter->last() == '{')
      location.limit_except = parseLimitExcept_(iter);
    else if (iter->firstWord() == "autoindex")
      location.autoindex = parseAutoindex(*iter);
    else if (iter->firstWord() == "client_max_body_size")
      location.client_max_body_size = parseCientMaxBodySize(*iter);
    else if (iter->firstWord() == "index")
      location.index = parseIndex(*iter);
    else if (iter->firstWord() == "root")
      location.root = parseRoot(*iter);
    else if (iter->firstWord() == "fastcgi_pass")
      location.fastcgi_pass.insert(parseFastcgiPass(*iter));
    else if (iter->firstWord() == "error_page")
      location.error_page.insert(parseErrorPage(*iter));
    else if (iter->getLine() == "}")
      break;
    else
      iter->addError("unknown option: " + iter->firstWord());
  }
  return location;
}

std::vector<std::string> ConfigFile::parseLimitExcept_(LineIter& iter) {
  std::vector<std::string> LimitExcept;
  std::stringstream ss(iter->getLine());
  std::string buffer;
  std::getline(ss, buffer, ' ');
  if (!(buffer == "limit_except")) {
    std::cout << "something weird happened in parsing limit except"
              << std::endl;
    exit(1);  // remove me
  }
  while (std::getline(ss, buffer, ' ')) {
    if (buffer == "}")
      break;
    else if (buffer == "GET" || buffer == "POST" || buffer == "DELETE")
      LimitExcept.push_back(buffer);
    else {
      std::cout << "wrong method in parse Limit Except" << std::endl;
      exit(1);  // remove me
    }
  }
  return LimitExcept;
}

std::ostream& operator<<(std::ostream& stream, const ConfigFile& config) {
  stream << "Line\tError\tContent\n";
  stream << std::boolalpha;
  for (std::vector<Line>::const_iterator iter = config.content_.begin();
       iter != config.content_.end(); ++iter) {
    stream << *iter << "\n";
  }
  stream << std::flush;
  return stream;
}
