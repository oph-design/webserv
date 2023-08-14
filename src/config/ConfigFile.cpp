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
  for (std::vector<Line>::iterator iter = this->content_.begin();
       iter != this->content_.end(); ++iter) {
    iter->trimWhitespace();
    iter->removeComment();
    if (!(*iter).isEmpty()) newContent.push_back(*iter);
  }
  this->content_ = newContent;
}

void ConfigFile::vaildateConfigFile() {
  this->checkSeparator();
  this->checkConfigBlocks();
}

bool ConfigFile::isValid() const {
  for (std::vector<Line>::const_iterator iter = this->content_.begin();
       iter != this->content_.end(); ++iter) {
    if (!iter->isValid()) return false;
  }
  return true;
}

void ConfigFile::checkSeparator() {
  for (std::vector<Line>::iterator iter = this->content_.begin();
       iter != this->content_.end(); ++iter) {
    if (iter->last() != ';' && iter->last() != '{' && iter->last() != '}')
      iter->addError("missing Separator");
  }
}

void ConfigFile::checkConfigBlocks() {
  std::list<std::vector<Line>::iterator> openings;
  for (std::vector<Line>::iterator iter = this->content_.begin();
       iter != this->content_.end(); ++iter) {
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
    for (std::list<std::vector<Line>::iterator>::iterator iter =
             openings.begin();
         iter != openings.end(); ++iter) {
      (*iter)->addError("Missing block closing");
    }
  }
}

std::vector<Config> ConfigFile::createConfig()
{
  std::vector<Config> configVector;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end(); ++iter) {
    if ((*iter).getLine() == "server {")
      configVector.push_back(parseServer_(iter));
  }
  return configVector;
}

Config ConfigFile::parseServer_(std::vector<Line>::iterator &iter)
{
  Config config;
  for (; iter != this->content_.end(); ++iter) {
    if ((*iter).getLine().compare(0, 8, "location ") == 0 && (*iter).last() == '{')
      config.locations.push_back(parseLocation_(iter));
    else if ((*iter).getLine() == "}")
      break;
  }
  return config;
}

Location ConfigFile::parseLocation_(std::vector<Line>::iterator &iter)
{
  Location location;
  for (; iter != this->content_.end(); ++iter) {
    if ((*iter).getLine().compare(0, 13, "limit_except ") == 0 && (*iter).last() == '{')
      location.limit_except = parseLimitExcept_(iter);
    else if ((*iter).getLine() == "}")
      break;
  }
  return location;
}

std::vector<std::string> ConfigFile::parseLimitExcept_(std::vector<Line>::iterator &iter)
{
  std::vector<std::string> LimitExcept;
  std::stringstream ss(iter->getLine());
  std::string buffer;
  std::getline(ss, buffer, ' ');
  if (!(buffer == "limit_except")){
    std::cout << "something weird happened in parsing limit except" << std::endl;
    exit(1);  // remove me
    }
  while (std::getline(ss, buffer, ' ')) {
    if (buffer == "}")
      break;
    else if (buffer == "GET" || buffer == "POST" || buffer == "DELETE")
      LimitExcept.push_back(buffer);
    else
    {
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
  if (config.isValid())
    stream << GREEN << "config is valid\n" << WHITE;
  else
    stream << RED << "config is invalid\n" << WHITE;
  stream << std::flush;
  return stream;
}
