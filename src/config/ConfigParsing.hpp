#ifndef CONFIGPARSING_HPP_
#define CONFIGPARSING_HPP_

#include <cstdlib>
#include <string>
#include <utility>

#include "Line.hpp"
#include "Location.hpp"

class ConfigParsing {
 public:
  // in Location and Server
  static int parseCientMaxBodySize(Line &line, t_duplicates &duplicates);
  static std::string parseIndex(Line &line, t_duplicates &duplicates);
  static std::string parseRoot(Line &line, t_duplicates &duplicates);
  static std::pair<int, std::string> parseErrorPage(Line &line);

  // in Server
  static int parseListen(Line &line);
  static std::string parseServerName(Line &line);
  static Location parseLocation(Line &line);

  // in Location
  static std::string parsePath(Line &line);
  static bool parseAutoindex(Line &line);
  static std::pair<std::string, std::string> parseFastcgiPass(Line &line);

 private:
  ConfigParsing();
  ~ConfigParsing();
};

#endif  // CONFIGPARSING_HPP_
