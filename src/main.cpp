#include <iostream>

#include "Header.hpp"

int main(void) {
  Header header;
  Header header2 = header;
  std::cout << header << std::endl;
  return 0;
}
