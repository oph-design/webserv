#include "Line.hpp"

Line::Line() : lineNumber_(0), error_(false) {}

Line::~Line() {}

Line::Line(const Line &obj) { *this = obj; }

Line::Line(int lineNumber, const std::string &content)
    : content_(content), lineNumber_(lineNumber), error_(false) {}

Line &Line::operator=(const Line &obj) {
  this->lineNumber_ = obj.lineNumber_;
  this->content_ = obj.content_;
  this->error_ = obj.error_;
  this->errorMessage_ = obj.errorMessage_;
  return *this;
}

std::string Line::operator[](const int &key) const {
  if (key >= this->words())
    throw std::out_of_range("Line " + toString(this->lineNumber_));
  int count = 0;
  std::stringstream ss(this->content_);
  std::string buffer;
  while (count <= key) {
    std::getline(ss, buffer, ' ');
    ++count;
  }
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
  if (pos != std::string::npos) this->content_ = this->content_.substr(0, pos);
}

bool Line::isEmpty() const { return this->content_.empty() ? true : false; }

bool Line::isValid() const { return !this->error_; }

std::ostream &operator<<(std::ostream &stream, const Line &line) {
  stream << line.lineNumber_ + 1 << ":\t" << line.error_ << ":\t"
         << line.content_;
  if (line.error_) {
    stream << RED << "\n" << line.errorMessage_ << WHITE;
  }
  stream << std::flush;
  return stream;
}

void Line::addError(const std::string &errorMessage) {
  this->error_ = true;
  this->errorMessage_.append(errorMessage + "\n");
}

const std::string &Line::getLine() const { return this->content_; }

const char &Line::last() const {
  if (!this->content_.empty())
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

void Line::removeSemiColon() {
  if (!this->content_.empty() && this->last() == ';')
    this->content_ = this->content_.substr(0, this->content_.size() - 1);
}

void Line::copyAllButContent(const Line &obj) {
  this->errorMessage_ = obj.errorMessage_;
  this->lineNumber_ = obj.lineNumber_;
  this->error_ = obj.error_;
}

std::string Line::lastWord() const {
  if (this->words() == 1) return this->firstWord();
  std::stringstream ss(this->content_);
  std::string buffer;
  std::string prior;
  while (std::getline(ss, buffer, ' ')) {
    if (buffer.empty()) break;
    prior = buffer;
  }
  return buffer;
}

const std::size_t &Line::getLineNumber() const { return this->lineNumber_; }
