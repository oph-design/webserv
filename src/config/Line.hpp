#ifndef LINE_HPP_
#define LINE_HPP_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ToString.hpp"
#include "colors.hpp"

class Line {
 public:
  Line();
  ~Line();
  Line(const Line& obj);
  Line(int lineNumber, std::string content);
  Line& operator=(const Line& obj);

  friend std::ostream& operator<<(std::ostream& stream, const Line& line);
  const std::string operator[](const int& key) const;

  void trimWhitespace();
  void removeComment();
  bool isEmpty() const;
  bool isValid() const;
  void addError(const std::string& errorMessage);
  const std::string& getLine() const;

  const std::size_t& getLineNumber() const;
  const char& last() const;
  std::string firstWord() const;
  std::string lastWord() const;
  int words() const;

  void removeSemiColon();

  void copyAllButContent(const Line& line);

 private:
  std::string content_;
  std::string errorMessage_;
  std::size_t lineNumber_;
  bool error_;
};

#endif  // LINE_HPP_
