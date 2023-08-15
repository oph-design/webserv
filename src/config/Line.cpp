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
  this->errorMessage_ = obj.error_;
  return *this;
}

const std::string Line::operator[](const int& key) const {
  if (key >= this->words())
    throw std::out_of_range("Line " + toString(this->lineNumber_));
  int count = 0;
  std::stringstream ss(this->content_);
  std::string buffer;
  while (std::getline(ss, buffer) && count <= key) ++count;
  return buffer;
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

bool Line::isValid() const { return !this->error_; }

std::ostream& operator<<(std::ostream& stream, const Line& line) {
  stream << line.lastWord() << " " << line.lineNumber_ << ":\t" << line.error_
         << ":\t" << line.content_;
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

const std::string& Line::getLine() const { return this->content_; }

const char& Line::last() const {
  if (this->content_.size() > 0)
    return *(this->content_.end() - 1);
  else
    return *(this->content_.end());
}

std::string Line::firstWord() const {
  std::stringstream ss(this->content_);
  std::string buffer;
  std::getline(ss, buffer, ' ');
  return buffer;
}

int Line::words() const {
  int wordCount = 0;
  std::stringstream ss(this->content_);
  std::string buffer;
  while (std::getline(ss, buffer, ' ')) ++wordCount;
  return wordCount;
}

std::string Line::lastWord() const {
  if (this->words() == 1) return this->firstWord();
  std::stringstream ss(this->content_);
  std::string buffer;
  std::string prior;
  while (std::getline(ss, buffer, ' ')) {
    if (buffer.size() == 0) break;
    prior = buffer;
  }
  return buffer;
}

const int& Line::getLineNumber() const { return this->lineNumber_; }
