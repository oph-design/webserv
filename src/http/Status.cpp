#include "Status.hpp"

#include <sstream>

#include "ToString.hpp"

statusMap Status::stats_;

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

void Status::fillStatusMap() {
  stats_.insert(statusPair(101, "Switching Protocols"));
  stats_.insert(statusPair(102, "Processing"));
  stats_.insert(statusPair(103, "Early Hints"));
  stats_.insert(statusPair(200, "OK"));
  stats_.insert(statusPair(201, "Created"));
  stats_.insert(statusPair(202, "Accepted"));
  stats_.insert(statusPair(203, "Non-Authoritative Information"));
  stats_.insert(statusPair(204, "No Content"));
  stats_.insert(statusPair(205, "Reset Content"));
  stats_.insert(statusPair(206, "Partial Content"));
  stats_.insert(statusPair(207, "Multi-Status"));
  stats_.insert(statusPair(208, "Already Reported"));
  stats_.insert(statusPair(226, "IM Used"));
  stats_.insert(statusPair(300, "Multiple Choices"));
  stats_.insert(statusPair(301, "Moved Permanently"));
  stats_.insert(statusPair(302, "Found"));
  stats_.insert(statusPair(303, "See Other"));
  stats_.insert(statusPair(304, "Not Modified"));
  stats_.insert(statusPair(305, "Use Proxy"));
  stats_.insert(statusPair(307, "Temporary Redirect"));
  stats_.insert(statusPair(308, "Permanent Redirect"));
  stats_.insert(statusPair(400, "Bad Request"));
  stats_.insert(statusPair(401, "Unauthorized"));
  stats_.insert(statusPair(402, "Payment Required"));
  stats_.insert(statusPair(403, "Forbidden"));
  stats_.insert(statusPair(404, "Not Found"));
  stats_.insert(statusPair(405, "Method Not Allowed"));
  stats_.insert(statusPair(406, "Not Acceptable"));
  stats_.insert(statusPair(407, "Proxy Authentication Required"));
  stats_.insert(statusPair(408, "Request Timeout"));
  stats_.insert(statusPair(409, "Conflict"));
  stats_.insert(statusPair(410, "Gone"));
  stats_.insert(statusPair(411, "Length Required"));
  stats_.insert(statusPair(412, "Precondition Failed"));
  stats_.insert(statusPair(413, "Payload Too Large"));
  stats_.insert(statusPair(414, "URI Too Long"));
  stats_.insert(statusPair(415, "Unsupported Media Type"));
  stats_.insert(statusPair(416, "Range Not Satisfiable"));
  stats_.insert(statusPair(417, "Expectation Failed"));
  stats_.insert(statusPair(418, "I'm a teapot"));
  stats_.insert(statusPair(421, "Misdirected Request"));
  stats_.insert(statusPair(422, "Unprocessable Entity"));
  stats_.insert(statusPair(423, "Locked"));
  stats_.insert(statusPair(424, "Failed Dependency"));
  stats_.insert(statusPair(425, "Too Early"));
  stats_.insert(statusPair(426, "Upgrade Required"));
  stats_.insert(statusPair(428, "Precondition Required"));
  stats_.insert(statusPair(429, "Too Many Requests"));
  stats_.insert(statusPair(431, "Request Header Fields Too Large"));
  stats_.insert(statusPair(451, "Unavailable For Legal Reasons"));
  stats_.insert(statusPair(501, "Not Implemented"));
  stats_.insert(statusPair(502, "Bad Gateway"));
  stats_.insert(statusPair(503, "Service Unavailable"));
  stats_.insert(statusPair(504, "Gateway Timeout"));
  stats_.insert(statusPair(505, "HTTP Version Not Supported"));
  stats_.insert(statusPair(506, "Variant Also Negotiates"));
  stats_.insert(statusPair(507, "Insufficient Storage"));
  stats_.insert(statusPair(508, "Loop Detected"));
  stats_.insert(statusPair(510, "Not Extended"));
  stats_.insert(statusPair(511, "Network Authentication Required"));
}
