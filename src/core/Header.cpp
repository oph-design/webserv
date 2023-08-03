#include "Header.hpp"

Header::Header() {
  this->headerContent_.insert(
      std::make_pair(std::string("key1"), std::string("content1")));
  this->headerContent_.insert(
      std::make_pair(std::string("key2"), std::string("content2")));
  this->headerContent_.insert(
      std::make_pair(std::string("key3"), std::string("content3")));
  this->headerContent_.insert(
      std::make_pair(std::string("key4"), std::string("content4")));
}

Header::Header(char buffer[BUFFER_SIZE]) {
  std::string string(buffer);
  *this = Header(string);
}

// Header::Header(std::string &bufferString) {
//   std::string::iterator iter = bufferString.begin();
//   for (; iter != bufferString.end(); ++iter) {
//     this->requestLine_ += *iter;
//     if ((int)*iter == 13 && (int)*std::next(iter) == 10) {
//       iter += 2;
//       break;
//     }
//   }
//   std::string line;
//   for (; iter < bufferString.end(); ++iter) {
//     if ((int)*iter == 13)
//       continue;
//     line += *iter;
//     if ((int)*iter == 10) {
//       iter += 2;
//       std::pair<std::string, std::string> keyPair;
//       keyPair.first = line.substr(0, line.find(": "));
//       keyPair.second = line.substr(line.find(": "));
//       this->headerContent_.insert(keyPair);
//       line.clear();
//     }
//   }
// }

Header::Header(std::string bufferString) {
  std::stringstream ss(bufferString);
  std::string firstLine;
  std::getline(ss, firstLine);
  firstLine.erase(std::remove(firstLine.begin(), firstLine.end(), '\n'), firstLine.end());
  firstLine.erase(std::remove(firstLine.begin(), firstLine.end(), '\r'), firstLine.end());
  while (true) {
    std::string line;
    std::getline(ss, line);
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    if (line.size() == 0)
      break ;
    std::pair<std::string, std::string> keyPair;
    keyPair.first = line.substr(0, line.find(": "));
    keyPair.second = line.substr(line.find(": ") + 2, line.end() - line.begin());
    this->headerContent_.insert(keyPair);
  }
}

Header::~Header() {}

Header::Header(const Header &obj) { *this = obj; }

Header &Header::operator=(const Header &obj) {
  this->headerContent_ = obj.headerContent_;
  return *this;
}

const std::string &Header::operator[](const std::string &key) const {
  return this->headerContent_.at(key);
}

const std::string &Header::operator[](const char *key) const {
  std::string stringKey(key);
  return this->headerContent_.at(stringKey);
}

std::ostream &operator<<(std::ostream &stream, const Header &header) {
  for (std::map<std::string, std::string>::const_iterator iter =
           header.headerContent_.begin();
       iter != header.headerContent_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    if (std::next(iter) != header.headerContent_.end()) stream << ", ";
  }
  return stream;
}
