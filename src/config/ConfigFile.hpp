#ifndef CONFIGFILE_HPP_
#define CONFIGFILE_HPP_

#ifndef STD_CONF_PATH
#define STD_CONF_PATH "./conf/webserv.conf"
#endif  // STD_CONF_PATH

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "ConfigParsing.hpp"
#include "Line.hpp"
#include "Location.hpp"
#include "Utils.hpp"
#include "colors.hpp"

class ConfigFile {
 public:
  ~ConfigFile();
  ConfigFile();
  ConfigFile(const ConfigFile &obj);
  ConfigFile &operator=(const ConfigFile &obj);
  bool openFile(int argc, char *argv[]);
  ConfigVector createConfig();
  bool isValid();
  void printError() const;
  bool isValid() const;

  friend std::ostream &operator<<(std::ostream &stream, ConfigFile &config);

 private:
  static ConfigVector splitUpListens_(ConfigVector &configvector);
  void cleanContent_();
  void validateConfigFile_();
  ConfigVector createConfigVector_();
  void removeSemiColon_();
  void checkSeparator_();
  void checkConfigBlocks_();

  void updateBackup();

  LineVector content_;
  LineVector backup_;
};

#endif  // CONFIGFILE_HPP_
