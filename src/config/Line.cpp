#include "Line.hpp"

Line::Line() : lineNumber_(0), error_(false) {}

Line::~Line() {}

Line::Line(const Line& obj) { *this = obj; }

Line::Line(int lineNumber, std::string content)
    : content_(content), lineNumber_(lineNumber), error_(false) {}

Line& Line::operator=(const Line& obj) {
  this->lineNumber_ = obj.lineNumber_;
  this->content_ = obj.content_;
  this->error_ = obj.error_;
  return *this;
}

void Line::trimWhitespace() {
  std::istringstream iss(this->content_);
  std::ostringstream oss;
  std::string word;
  bool firstWord = true;
  while (iss >> word) {
    if (!firstWord) oss << ' ';
    oss << word;
    firstWord = false;
  }
  this->content_ = oss.str();
}

void Line::removeComment() {
  std::size_t pos = this->content_.find('#');
  if (pos != this->content_.npos)
    this->content_ = this->content_.substr(0, pos);
}

bool Line::isEmpty() const {
  if (this->content_.size() == 0)
    return true;
  else
    return false;
}

std::ostream& operator<<(std::ostream& stream, const Line& line) {
  stream << line.lineNumber_ << ":\t" << line.error_ << ":\t" << line.content_;
  if (line.error_ == true) {
    stream << "\n" << line.errorMessage_;
  }
  stream << std::flush;
  return stream;
}

void Line::addError(std::string errorMessage) {
  this->error_ = true;
  this->errorMessage_.append(errorMessage + "\n");
}

const char& Line::last() const {
  if (this->content_.size() > 0)
    return *(this->content_.end() - 1);
  else
    return *(this->content_.end());
}
