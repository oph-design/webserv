
#ifndef PRINTVERBOSE_HPP_
#define PRINTVERBOSE_HPP_

#include <iostream>
#define VERBOSE false

template <typename Type>
void printVerbose(std::string call, Type val) {
  if (!VERBOSE) return;
  std::cout << call << val << std::endl;
}
#endif  // PRINTVERBOSE_HPP_
