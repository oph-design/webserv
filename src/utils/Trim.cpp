#include "Trim.hpp"

std::string ltrim(const std::string &s, std::string trim) {
  size_t start = s.find_first_not_of(trim);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s, std::string trim) {
  size_t end = s.find_last_not_of(trim);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s, std::string trim) {
  return rtrim(ltrim(s, trim), trim);
}
