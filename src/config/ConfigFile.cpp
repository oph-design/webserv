#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::~ConfigFile() {}

ConfigFile::ConfigFile(const ConfigFile& obj) { *this = obj; }

ConfigFile& ConfigFile::operator=(const ConfigFile& obj) {
  this->content_ = obj.content_;
  this->backup_ = obj.backup_;
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
  this->backup_ = this->content_;
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

bool lineExistsInLineVector(LineVector lineVector, std::size_t lineNbr) {
  for (LineIter iter = lineVector.begin(); iter != lineVector.end(); ++iter) {
    if (iter->getLineNumber() == lineNbr) return true;
  }
  return false;
}

void ConfigFile::updateBackup() {
  LineVector newBackup = this->content_;
  for (LineIter iter = this->backup_.begin(); iter != this->backup_.end();
       ++iter) {
    if (lineExistsInLineVector(this->content_, iter->getLineNumber()))
      continue;
    else {
      if (newBackup.size() > iter->getLineNumber())
        newBackup.insert(newBackup.begin() + iter->getLineNumber(), *iter);
      else
        newBackup.push_back(*iter);
    }
  }
  this->backup_ = newBackup;
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
  if (this->isValid() == false) return ConfigVector();
  ConfigVector configVector = ConfigFile::createConfigVector_();
  if (this->isValid() == false) return ConfigVector();
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
    else if (iter->getLine() == "}")
      break;
    else
      iter->addError("server: unknown argument");
  }
  return configVector;
}

std::ostream& operator<<(std::ostream& stream, ConfigFile& config) {
  config.updateBackup();
  stream << "Line\tError\tContent\n";
  stream << std::boolalpha;
  for (LineVector::iterator iter = config.backup_.begin();
       iter != config.backup_.end(); ++iter) {
    stream << *iter << "\n";
  }
  stream << std::flush;
  return stream;
}
