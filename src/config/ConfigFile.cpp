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

void ConfigFile::cleanContent_() {
  LineVector newContent;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    iter->trimWhitespace();
    iter->removeComment();
    if (!iter->isEmpty()) newContent.push_back(*iter);
  }
  this->content_ = newContent;
}

void ConfigFile::vaildateConfigFile_() {
  this->checkSeparator_();
  this->checkConfigBlocks_();
}

bool ConfigFile::isValid() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (!iter->isValid()) return false;
  }
  return true;
}

void ConfigFile::checkSeparator_() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->last() != ';' && iter->last() != '{' && iter->last() != '}')
      iter->addError("missing Separator");
  }
}

void ConfigFile::checkConfigBlocks_() {
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

void ConfigFile::removeSemiColon_() {
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->last() == ';') iter->removeSemiColon();
  }
}

ConfigVector ConfigFile::createConfig() {
  this->cleanContent_();
  this->vaildateConfigFile_();
  this->removeSemiColon_();
  ConfigVector configVector = ConfigFile::createConfigVector_();
  configVector = Config::handleDuplicates_(configVector);
  return configVector;
}

ConfigVector ConfigFile::createConfigVector_() {
  ConfigVector configVector;
  for (LineIter iter = this->content_.begin(); iter != this->content_.end();
       ++iter) {
    if (iter->getLine() == "server {")
      configVector.push_back(
          ConfigParsing::parseServer_(iter, this->content_.end()));
    else
      iter->addError("server: unknown argument");
  }
  return configVector;
}

std::ostream& operator<<(std::ostream& stream, const ConfigFile& config) {
  stream << "Line\tError\tContent\n";
  stream << std::boolalpha;
  for (LineVector::const_iterator iter = config.content_.begin();
       iter != config.content_.end(); ++iter) {
    stream << *iter << "\n";
  }
  stream << std::flush;
  return stream;
}
