#include "getContentDisposition.hpp"

#include <stdexcept>

std::string getContentDispostion(const Request& request, std::string key) {
  std::string value;
  try {
    value = request["Content-Disposition"];
    if (value.find(key) == key.npos) throw std::runtime_error("no " + key);
    value = value.substr(value.find(key), value.length());
    value = value.substr(value.find("=") + 1, value.length());
    value = trim(value.substr(0, value.find(";")), "\'\"");
  } catch (std::exception& e) {
    throw e;
  }
  return (value);
}
