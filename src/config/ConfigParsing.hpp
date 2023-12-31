#ifndef CONFIGPARSING_HPP_
#define CONFIGPARSING_HPP_

#include <climits>
#include <cstdlib>
#include <string>
#include <utility>

#include "Config.hpp"
#include "Line.hpp"
#include "Location.hpp"
#include "Utils.hpp"

class ConfigParsing {
 public:
  // Blocks
  static std::set<std::string> parseLimitExcept_(LineIter &iter,
                                                 const LineIter &end);
  static Location parseLocation_(LineIter &iter, const LineIter &end);
  static Config parseServer_(LineIter &iter, const LineIter &end);

  // in Location and Server
  static int parseClientMaxBodySize(Line &line, Duplicates &duplicates);
  static std::string parseIndex(Line &line, Duplicates &duplicates);
  static std::string parseRoot(Line &line, Duplicates &duplicates);
  static std::string parseUpload(Line &line, Duplicates &duplicates);
  static ErrorPage parseErrorPage(Line &line);

  // in Server
  static int parseListen(Line &line);
  static std::string parseServerName(Line &line);
  static int parseTimeout(Line &line);

  // in Location
  static std::string parsePath(Line &line);
  static std::string parseRewrite(Line &line);
  static bool parseAutoindex(Line &line);
  static bool parseCgiProcessing(Line &line);
  static std::string parseCgiPass(Line &line, Duplicates &duplicates);

 private:
  ConfigParsing();
  ~ConfigParsing();
};

#endif  // CONFIGPARSING_HPP_
