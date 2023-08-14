#ifndef CONFIGFILE_HPP_
#define CONFIGFILE_HPP_

#ifndef STD_CONF_PATH
#define STD_CONF_PATH "./conf/webserv.conf"
#endif  // STD_CONF_PATH

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <sstream>

#include "Line.hpp"
#include "colors.hpp"
#include "Config.hpp"
#include "Types.hpp"
#include "Location.hpp"

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

  bool isValid() const;

  friend std::ostream& operator<<(std::ostream& stream,
                                  const ConfigFile& config);

 private:
  void checkSeparator();
  void checkConfigBlocks();

  Config parseServer_(std::vector<Line>::iterator &iter);
  Location parseLocation_(std::vector<Line>::iterator &iter);
  std::vector<std::string> parseLimitExcept_(std::vector<Line>::iterator &iter);

  std::vector<Line> content_;
};

#endif  // CONFIGFILE_HPP_
