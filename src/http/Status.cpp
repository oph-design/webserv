#include "Status.hpp"

statusMap Status::stats_ = Status::createStatusMap();

Status::Status() : code_(200) {}

Status::Status(const Status &rhs) { *this = rhs; }

Status &Status::operator=(const Status &rhs) {
  this->code_ = rhs.code_;
  return (*this);
}

Status &Status::operator=(int newCode) {
  this->code_ = newCode;
  return (*this);
}

Status::~Status() {}

void Status::setErrors(const Location &location) {
  this->root_ = location.getRoot();
  this->errors_ = location.getErrorPage();
}

std::ostream &operator<<(std::ostream &stream, const Status &status) {
  stream << toString<int>(status.code_) << " " << Status::stats_[status.code_];
  return (stream);
}

bool Status::operator==(int rhs) const { return (this->code_ == rhs); }

bool Status::operator<=(int rhs) const { return (this->code_ <= rhs); }

bool Status::operator>=(int rhs) const { return (this->code_ >= rhs); }

bool Status::operator>(int rhs) const { return (this->code_ > rhs); }

bool Status::operator<(int rhs) const { return (this->code_ < rhs); }

std::string Status::getStdError() {
  std::string path = this->root_ + "/" + this->errors_[this->code_];
  std::ifstream error(path.c_str());
  if (!error.is_open())
    return "";
  std::stringstream content;
  content << error.rdbuf();
  error.close();
  return (content.str());
}

std::string &Status::operator>>(std::string &str) {
  std::stringstream body;
  str = this->getStdError();
  if (!str.empty())
    return (str);
  body << "<!DOCTYPE html>\n";
  body << "<html lang=\"en\">\n";
  body << "<head>\n";
  body << "\t<meta charset=\"UTF-8\">\n";
  body << "\t<meta name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1.0\">\n";
  body << "\t<title>" << *this << "</title>\n";
  body << "<link rel=\"stylesheet\" href=\"error.css\">\n";
  body << "</head>\n";
  body << "<body>\n";
  body << "\t<div class=\"container\">\n";
  body << "\t\t<h1>Error " << code_ << "</h1>\n";
  body << "\t\t<p>Oops! Looks like your grind wasn't hard enough!</p>\n";
  body << "\t\t<p>Please check the URL or go back to the <a "
          "href=\"/\">homepage</a>.</p>\n";
  body << "\t</div>\n";
  body << "</body>";
  body << "</html>";
  str = body.str();
  return (str);
}

statusMap Status::createStatusMap() {
  std::ifstream data("./data/statuscodes.csv");
  std::string field;
  std::string key;
  std::string value;
  statusMap stats;

  while (data.is_open() && std::getline(data, field)) {
    try {
      key = field.substr(0, field.find(','));
      value = field.substr(field.find(',') + 1);
    } catch (std::exception &) {
      continue;
    }
    stats.insert(statusPair(std::atoi(key.c_str()), value));
  }
  return (stats);
}
