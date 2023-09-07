#include "ConfigParsing.hpp"

Config ConfigParsing::parseServer_(LineIter &iter, const LineIter &end) {
  Config config;
  Duplicates duplicates;
  ++iter;
  for (; iter != end; ++iter) {
    if (iter->firstWord() == "location" && iter->last() == '{')
      config.locations_.push_back(ConfigParsing::parseLocation_(iter, end));
    else if (iter->firstWord() == "listen")
      config.listen_.insert(ConfigParsing::parseListen(*iter));
    else if (iter->firstWord() == "client_max_body_size")
      config.clientMaxBodySize_ =
          ConfigParsing::parseClientMaxBodySize(*iter, duplicates);
    else if (iter->firstWord() == "server_name")
      config.serverName_ = ConfigParsing::parseServerName(*iter);
    else if (iter->firstWord() == "index")
      config.index_ = ConfigParsing::parseIndex(*iter, duplicates);
    else if (iter->firstWord() == "root")
      config.root_ = ConfigParsing::parseRoot(*iter, duplicates);
    else if (iter->firstWord() == "error_page")
      config.errorPage_.insert(ConfigParsing::parseErrorPage(*iter));
    else if (iter->firstWord() == "timeout")
      config.timeout_ = ConfigParsing::parseTimeout(*iter);
    else if (iter->getLine() == "}")
      break;
    else
      iter->addError("unknown option" + iter->firstWord());
  }
  return config;
}

Location ConfigParsing::parseLocation_(LineIter &iter, const LineIter &end) {
  Location location;
  Duplicates duplicates;
  location.path_ = ConfigParsing::parsePath(*iter);
  ++iter;
  for (; iter != end; ++iter) {
    if (iter->firstWord() == "limit_except" && iter->last() == '{')
      location.limitExcept_ = ConfigParsing::parseLimitExcept_(iter, end);
    else if (iter->firstWord() == "autoindex")
      location.autoindex_ = ConfigParsing::parseAutoindex(*iter);
    else if (iter->firstWord() == "cgi_processing")
      location.cgiProcessing_ = ConfigParsing::parseCgiProcessing(*iter);
    else if (iter->firstWord() == "client_max_body_size")
      location.clientMaxBodySize_ =
          ConfigParsing::parseClientMaxBodySize(*iter, duplicates);
    else if (iter->firstWord() == "index")
      location.index_ = ConfigParsing::parseIndex(*iter, duplicates);
    else if (iter->firstWord() == "root")
      location.root_ = ConfigParsing::parseRoot(*iter, duplicates);
    else if (iter->firstWord() == "upload_pass")
      location.uploadPass_ = ConfigParsing::parseUpload(*iter, duplicates);
    else if (iter->firstWord() == "rewrite")
      location.rewrite_ = ConfigParsing::parseRewrite(*iter);
    else if (iter->firstWord() == "fastcgi_pass")
      location.cgiPass_ = ConfigParsing::parseCgiPass(*iter, duplicates);
    else if (iter->firstWord() == "error_page")
      location.errorPage_.insert(ConfigParsing::parseErrorPage(*iter));
    else if (iter->getLine() == "}")
      break;
    else
      iter->addError("unknown option: " + iter->firstWord());
  }
  location.duplicates_ = duplicates;
  return location;
}

StringSet ConfigParsing::parseLimitExcept_(LineIter &iter,
                                           const LineIter &end) {
  StringSet LimitExcept;
  for (int lineIter = 1; lineIter < iter->words() - 1; ++lineIter) {
    if ((*iter)[lineIter] == "GET" || (*iter)[lineIter] == "POST" ||
        (*iter)[lineIter] == "DELETE")
      LimitExcept.insert((*iter)[lineIter]);
  }
  ++iter;
  for (; iter != end; ++iter) {
    if (iter->getLine() == "}")
      break;
    else if (iter->getLine() == "deny all")
      continue;
    else
      iter->addError("line_except unknown token");
  }
  return LimitExcept;
}

int ConfigParsing::parseListen(Line &line) {
  std::string parameter = "listen";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected port");
    return 0;
  }
  return std::atoi(line[1].c_str());
}

int ConfigParsing::parseTimeout(Line &line) {
  std::string parameter = "timeout";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected timeout");
    return 0;
  }
  return std::atoi(line[1].c_str());
}

int ConfigParsing::parseClientMaxBodySize(Line &line, Duplicates &duplicates) {
  std::string parameter = "client_max_body_size";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected number");
    return 0;
  }
  duplicates.clientMaxBodySize = true;
  return std::atoi(line[1].c_str());
}

std::string ConfigParsing::parseServerName(Line &line) {
  std::string parameter = "server_name";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  return line[1];
}

std::string ConfigParsing::parseIndex(Line &line, Duplicates &duplicates) {
  std::string parameter = "index";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  duplicates.index = true;
  std::string str = line[1];
  if (line[1][0] != '/')
    str = "/" + str;
  return str;
}

std::string ConfigParsing::parseRoot(Line &line, Duplicates &duplicates) {
  std::string parameter = "root";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  duplicates.root = true;
  std::string str = line[1];
  if (last(line[1]) == '/')
    str = str.substr(0, str.size() - 1);
  return str;
}

std::string ConfigParsing::parseUpload(Line &line, Duplicates &duplicates) {
  std::string parameter = "upload_pass";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  duplicates.upload_pass = true;
  std::string str = line[1];
  if (last(line[1]) == '/')
    str = str.substr(0, str.size() - 1);
  return str;
}

ErrorPage ConfigParsing::parseErrorPage(Line &line) {
  std::string parameter = "error_page";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return std::make_pair(0, "");
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected number");
    return std::make_pair(0, "");
  }
  return std::make_pair(std::atoi(line[1].c_str()), line[2]);
}

bool ConfigParsing::parseAutoindex(Line &line) {
  std::string parameter = "autoindex";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return false;
  }
  if (line[1] == "on") {
    return true;
  } else if (line[1] == "off") {
    return false;
  } else {
    line.addError(parameter + " unexpected option");
  }
  return false;
}

bool ConfigParsing::parseCgiProcessing(Line &line) {
  std::string parameter = "cgi_processing";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return false;
  }
  if (line[1] == "on") {
    return true;
  } else if (line[1] == "off") {
    return false;
  } else {
    line.addError(parameter + " unexpected option");
  }
  return false;
}

std::string ConfigParsing::parsePath(Line &line) {
  std::string parameter = "location path";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  std::string str = line[1];
  if (last(line[1]) == '/')
    str = str.substr(0, str.size() - 1);
  if (line[1][0] != '/')
    str = "/" + str;
  return str;
}

std::string ConfigParsing::parseRewrite(Line &line) {
  std::string parameter = "rewrite";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  return line[1];
}

std::string ConfigParsing::parseCgiPass(Line &line, Duplicates &duplicates) {
  std::string parameter = "cgi_pass";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return "";
  }
  duplicates.cgi_pass = true;
  std::string str = line[1];
  if (last(line[1]) == '/')
    str = str.substr(0, str.size() - 1);
  return str;
}
