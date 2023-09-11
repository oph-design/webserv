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
  if (!error.is_open()) return "";
  std::stringstream content;
  content << error.rdbuf();
  error.close();
  return (content.str());
}

std::string &Status::operator>>(std::string &str) {
  std::stringstream body;
  str = this->getStdError();
  if (!str.empty()) return (str);
  body << "<!DOCTYPE html>\n";
  body << "<html lang=\"en\">\n";
  body << "<head>\n";
  body << "\t<meta charset=\"UTF-8\">\n";
  body << "\t<meta name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1.0\">\n";
  body << getStyling_();
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

std::string Status::getStyling_() {
  std::stringstream style;
  style << "\t<style>\n";
  style << "\t\tbody {\n";
  style << "\t\t\tfont-family: Arial, Helvetica, sans-serif;\n";
  style << "\t\t\tbackground-color: #f5f5f5;\n";
  style << "\t\t\tmargin: 0;\n";
  style << "\t\t\tpadding: 0;\n";
  style << "\t\t\ttext-align: center;\n";
  style << "\t\t}\n";
  style << "\t\t.container {\n";
  style << "\t\t\tpadding: 20px;\n";
  style << "\t\t\tmax-width: 600px;\n";
  style << "\t\t\tmargin: 50px auto;\n";
  style << "\t\t\tborder: 1px solid #ddd;\n";
  style << "\t\t\tborder-radius: 5px;\n";
  style << "\t\t}\n";
  style << "\t\th1 {\n";
  style << "\t\t\tcolor: #d81b60;\n";
  style << "\t}\n";
  style << "\t\tp {\n";
  style << "\t\t\tcolor: #777;\n";
  style << "\t\t}\n";
  style << "\t\ta {\n";
  style << "\t\t\ttext-decoration: none;\n";
  style << "\t\t\tcolor: #4caf50;\n";
  style << "\t\t}\n";
  style << "\t\ta:hover {\n";
  style << "\t\t\ttext-decoration: underline;\n";
  style << "\t\t}\n";
  style << "\t</style>\n";
  return (style.str());
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
