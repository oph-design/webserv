#include "Utils.hpp"

template <typename Type>
void printVerbose(const std::string &call, Type val) {
  if (!VERBOSE) return;
  std::cout << call << val << COLOR_RESET << std::endl;
}

