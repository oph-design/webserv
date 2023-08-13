#include "Status.hpp"

#include <sstream>

#include "ToString.hpp"

statusMap Status::stats_ = Status::createStatusMap();

Status::Status() : code_(200) {}

Status::Status(const Status& rhs) { *this = rhs; }

Status& Status::operator=(const Status& rhs) {
  this->code_ = rhs.code_;
  return (*this);
}

Status::~Status() {}

std::ostream& operator<<(std::ostream& stream, const Status& status) {
  stream << toString<int>(status.code_) << " " << status.stats_[status.code_];
  return (stream);
}

const int& Status::getCode() const { return (code_); }

void Status::setCode(const int& newCode) { this->code_ = newCode; }

std::string Status::getErrorBody() const {
  std::stringstream body;
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
  return (body.str());
}

statusMap Status::createStatusMap() {
  statusMap stats;
  stats.insert(statusPair(101, "Switching Protocols"));
  stats.insert(statusPair(102, "Processing"));
  stats.insert(statusPair(103, "Early Hints"));
  stats.insert(statusPair(200, "OK"));
  stats.insert(statusPair(201, "Created"));
  stats.insert(statusPair(202, "Accepted"));
  stats.insert(statusPair(203, "Non-Authoritative Information"));
  stats.insert(statusPair(204, "No Content"));
  stats.insert(statusPair(205, "Reset Content"));
  stats.insert(statusPair(206, "Partial Content"));
  stats.insert(statusPair(207, "Multi-Status"));
  stats.insert(statusPair(208, "Already Reported"));
  stats.insert(statusPair(226, "IM Used"));
  stats.insert(statusPair(300, "Multiple Choices"));
  stats.insert(statusPair(301, "Moved Permanently"));
  stats.insert(statusPair(302, "Found"));
  stats.insert(statusPair(303, "See Other"));
  stats.insert(statusPair(304, "Not Modified"));
  stats.insert(statusPair(305, "Use Proxy"));
  stats.insert(statusPair(307, "Temporary Redirect"));
  stats.insert(statusPair(308, "Permanent Redirect"));
  stats.insert(statusPair(400, "Bad Request"));
  stats.insert(statusPair(401, "Unauthorized"));
  stats.insert(statusPair(402, "Payment Required"));
  stats.insert(statusPair(403, "Forbidden"));
  stats.insert(statusPair(404, "Not Found"));
  stats.insert(statusPair(405, "Method Not Allowed"));
  stats.insert(statusPair(406, "Not Acceptable"));
  stats.insert(statusPair(407, "Proxy Authentication Required"));
  stats.insert(statusPair(408, "Request Timeout"));
  stats.insert(statusPair(409, "Conflict"));
  stats.insert(statusPair(410, "Gone"));
  stats.insert(statusPair(411, "Length Required"));
  stats.insert(statusPair(412, "Precondition Failed"));
  stats.insert(statusPair(413, "Payload Too Large"));
  stats.insert(statusPair(414, "URI Too Long"));
  stats.insert(statusPair(415, "Unsupported Media Type"));
  stats.insert(statusPair(416, "Range Not Satisfiable"));
  stats.insert(statusPair(417, "Expectation Failed"));
  stats.insert(statusPair(418, "I'm a teapot"));
  stats.insert(statusPair(421, "Misdirected Request"));
  stats.insert(statusPair(422, "Unprocessable Entity"));
  stats.insert(statusPair(423, "Locked"));
  stats.insert(statusPair(424, "Failed Dependency"));
  stats.insert(statusPair(425, "Too Early"));
  stats.insert(statusPair(426, "Upgrade Required"));
  stats.insert(statusPair(428, "Precondition Required"));
  stats.insert(statusPair(429, "Too Many Requests"));
  stats.insert(statusPair(431, "Request Header Fields Too Large"));
  stats.insert(statusPair(451, "Unavailable For Legal Reasons"));
  stats.insert(statusPair(501, "Not Implemented"));
  stats.insert(statusPair(502, "Bad Gateway"));
  stats.insert(statusPair(503, "Service Unavailable"));
  stats.insert(statusPair(504, "Gateway Timeout"));
  stats.insert(statusPair(505, "HTTP Version Not Supported"));
  stats.insert(statusPair(506, "Variant Also Negotiates"));
  stats.insert(statusPair(507, "Insufficient Storage"));
  stats.insert(statusPair(508, "Loop Detected"));
  stats.insert(statusPair(510, "Not Extended"));
  stats.insert(statusPair(511, "Network Authentication Required"));
  return (stats);
}
