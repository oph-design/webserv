#include "Line.hpp"

Line::Line() : lineNumber_(0), error_(false) {}

Line::~Line() {}

Line::Line(const Line& obj) { *this = obj; }

Line::Line(int lineNumber, std::string content)
    : lineNumber_(lineNumber), content_(content), error_(false) {}

Line& Line::operator=(const Line& obj) {
  this->lineNumber_ = obj.lineNumber_;
  this->content_ = obj.content_;
  this->error_ = obj.error_;
  return *this;
}
