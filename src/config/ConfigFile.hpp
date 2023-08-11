#ifndef CONFIGFILE_HPP_
#define CONFIGFILE_HPP_

#ifndef STD_CONF_PATH
#define STD_CONF_PATH "./conf/webserv.conf"
#endif  // STD_CONF_PATH

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "Line.hpp"

class ConfigFile {
 public:
  ~ConfigFile();
  ConfigFile();
  ConfigFile(const ConfigFile& obj);
  ConfigFile& operator=(const ConfigFile& obj);

  bool openFile(int argc, char* argv[]);
  void cleanContent();
  void vaildateConfigFile();

  friend std::ostream& operator<<(std::ostream& stream,
                                  const ConfigFile& config);

 private:
  void checkSeparator();
  void checkConfigBlocks();

  std::vector<Line> content_;
};

#endif  // CONFIGFILE_HPP_
