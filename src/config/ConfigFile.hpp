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
#include "Types.hpp"
#include "colors.hpp"

class ConfigFile {
 public:
  ~ConfigFile();
  ConfigFile();
  ConfigFile(const ConfigFile& obj);
  ConfigFile& operator=(const ConfigFile& obj);

  bool openFile(int argc, char* argv[]);

  ConfigVector createConfig();

  bool isValid();

  friend std::ostream& operator<<(std::ostream& stream,
                                  const ConfigFile& config);

 private:
  void cleanContent_();
  void vaildateConfigFile_();
  ConfigVector createConfigVector_();
  void removeSemiColon_();
  void checkSeparator_();
  void checkConfigBlocks_();

  Config parseServer_(LineIter& iter, const LineIter& end);
  Location parseLocation_(LineIter& iter, const LineIter& end);
  StringSet parseLimitExcept_(LineIter& iter, const LineIter& end);

  LineVector content_;
};

#endif  // CONFIGFILE_HPP_
