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
  void cleanContent();
  void vaildateConfigFile();

  std::vector<Config> createConfig();

  std::vector<Config> createConfigVector();

  bool isValid();

  friend std::ostream& operator<<(std::ostream& stream,
                                  const ConfigFile& config);

  void removeSemiColon();

 private:
  void checkSeparator();
  void checkConfigBlocks();

  Config parseServer_(LineIter& iter, const LineIter& end);
  Location parseLocation_(LineIter& iter, const LineIter& end);
  std::set<std::string> parseLimitExcept_(LineIter& iter, const LineIter& end);

  std::vector<Line> content_;
};

#endif  // CONFIGFILE_HPP_
