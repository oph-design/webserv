#include "GetContentDisposition.hpp"

#include <stdexcept>

std::string getContentDisposition(const Request &request,
                                  const std::string &key) {
  std::string value;
  try {
    value = request["Content-Disposition"];
    if (value.find(key) == std::string::npos)
      throw std::runtime_error("no " + key);
    value = value.substr(value.find(key), value.length());
    value = value.substr(value.find('=') + 1, value.length());
    value = trim(value.substr(0, value.find(';')), "\'\"");
  } catch (std::exception &e) {
    throw e;
  }
  return (value);
}
