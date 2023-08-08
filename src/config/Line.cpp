#include "Line.hpp"

Line::Line() {}

Line::~Line() {}

Line::Line(const Line& obj) { *this = obj; }

Line& Line::operator=(const Line& obj) {
  (void)obj;
  return *this;
}
