#ifndef LINE_HPP_
#define LINE_HPP_

#include <iostream>
#include <sstream>
#include <string>

class Line {
 public:
  Line();
  ~Line();
  Line(const Line& obj);
  Line(int lineNumber, std::string content);
  Line& operator=(const Line& obj);

  friend std::ostream& operator<<(std::ostream& stream, const Line& line);

  void trimWhitespace();
  void removeComment();
  bool isEmpty() const;
  bool isValid() const;
  void addError(std::string errorMessage);

  const int& getLineNumber() const;
  const char& last() const;

 private:
  std::string content_;
  std::string errorMessage_;
  int lineNumber_;
  bool error_;
};

#endif  // LINE_HPP_
