#include "getContentDisposition.hpp"

std::string getContentDispostion(const Request& request, std::string key) {
  std::string dispo;
  std::string value;
  try {
    dispo = request["Content-Disposition"];
  } catch (std::exception& e) {
    throw e;
  }
  try {
    value = dispo.substr(dispo.find(key), dispo.length());
    value = dispo.substr(0, dispo.find("="));
    value = dispo.substr(0, dispo.find(";"));
  } catch (std::exception& e) {
    throw e;
  }
  return (value);
}
