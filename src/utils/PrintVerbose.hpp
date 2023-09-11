
#ifndef PRINTVERBOSE_HPP_
#define PRINTVERBOSE_HPP_

#include <iostream>

#include "colors.hpp"

#define VERBOSE true

template <typename Type>
void printVerbose(std::string call, Type val) {
  if (!VERBOSE) return;
  std::cout << call << val << COLOR_RESET << std::endl;
}

#endif  // PRINTVERBOSE_HPP_
