#include "Response.hpp"

contentMap Response::fileTypes_ = Response::createTypeMap();

/*            constructors                  */

Response::Response() : body_("Server is online") {
  this->header_.insert(contentField("Content-Type", "text/plain"));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", "16"));
}

Response::Response(const Request &request) {
  switch (request.getRequestMethodType()) {
    case 0:
      handleGetRequest_(request);
      break;
    case 1:
      handlePostRequest_(request);
      break;
    case 2:
      handleDeleteRequest_(request);
      break;
    default:
      this->status_ = 405;
      this->status_ >> this->body_;
  }
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->status_ = rhs.status_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->header_.clear(); }

/*            operators                  */

const std::string &Response::operator[](const std::string &key) {
  return (this->header_[key]);
}

const std::string &Response::operator[](const char *key) {
  return ((*this)[std::string(key)]);
}

std::ostream &operator<<(std::ostream &stream, const Response &resp) {
  stream << resp.getHeader() << resp.getBody();
  return (stream);
}

/*            Getters           */

const std::string Response::getHeader() const {
  std::stringstream header;

  header << "HTTP/1.1 " << this->status_ << "\r\n";
  for (contentMap::const_iterator it = this->header_.begin();
       it != this->header_.end(); ++it)
    header << it->first << ": " << it->second << "\r\n";
  header << "\r\n";
  return (header.str());
}

const std::string &Response::getBody() const { return (body_); }

/*             Get Request                  */

std::string Response::findType_(std::string url) {
  std::string extention;
  contentMap::iterator search;
  std::string type;

  if (!url.compare("/")) url.append("index.html");
  extention = url.substr(url.rfind(".") + 1, url.length());
  search = Response::fileTypes_.find(extention);
  if (search != Response::fileTypes_.end()) {
    type = Response::fileTypes_[extention];
  } else {
    this->status_ = 415;
    type = "text/html";
  }
  type.append("; charset=UTF-8");
  return (type);
}

std::string Response::readBody_(std::string dir) {
  if (!dir.compare("/")) dir.append("index.html");
  std::ifstream file(("./html" + dir).c_str());

  if (file.is_open()) {
    std::cout << "File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open file." << std::endl;
    this->status_ = 404;
  }
  std::stringstream content;
  content << file.rdbuf();
  file.close();
  return (content.str());
}

void Response::handleGetRequest_(const Request &request) {
  if (CgiConnector::isCgi(request.getPath())) return (void)(serveCgi_(request));
  this->body_ = readBody_(request.getPath());
  std::string type = findType_(request.getPath());
  if (this->status_ > 399) this->status_ >> this->body_;
  std::string length = toString<std::size_t>(this->body_.length());

  this->header_.insert(contentField("Content-Type", type));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", length));
}

/*             Post Request                  */

void Response::createFile_(std::string filename, std::string ext,
                           std::string data, std::string path) {
  path = ".hmtl" + path.substr(0, path.rfind("/"));
  std::string file = filename + "." + ext;
  if (findFile(file, path))
    this->status_ = 200;
  else
    this->status_ = 201;
  std::ofstream outfile(path + file);
  if (!outfile.is_open()) this->status_ = 500;
  outfile << data;
}

void Response::buildJsonBody_() {
  this->body_ = "{\n";
  this->body_.append("\"status\": ");
  if (this->status_ < 400)
    this->body_.append("\"success\",\n");
  else
    this->body_.append("\"error\",\n");
  this->body_.append("}");
}

void Response::handlePostRequest_(const Request &request) {
  if (CgiConnector::isCgi(request.getPath())) return (void)(serveCgi_(request));
  std::string file = request.getPath();
  std::string ext;
  file = file.substr(file.rfind("/") + 1, file.length());
  file = file.substr(0, file.rfind("."));
  try {
    ext = swapColumns(fileTypes_)[request["Content-Type"]];
    file = getContentDispostion(request, "filename");
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  if (!request.getRequestBodyExists() || !file.length() || !ext.length())
    this->status_ = 400;
  if (this->status_ < 400)
    this->createFile_(file, ext, request.getRequestBody(), request.getPath());
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Type", "application/json"));
  buildJsonBody_();
}

/*             Delete Request                  */

void Response::handleDeleteRequest_(const Request &request) {
  if (CgiConnector::isCgi(request.getPath())) return (void)(serveCgi_(request));
  std::string path = "./html" + request.getPath();
  std::cout << path << std::endl;
  if (access(path.c_str(), F_OK)) this->status_ = 403;
  std::cout << this->status_ << std::endl;
  if (!findFile(path.substr(path.rfind("/") + 1, path.length()),
                path.substr(0, path.rfind("/"))))
    this->status_ = 404;
  std::cout << this->status_ << std::endl;
  if (this->status_ < 400 && remove(path.c_str())) this->status_ = 500;
  std::cout << this->status_ << std::endl;
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Type", "application/json"));
  buildJsonBody_();
}

/*             Cgi Request                  */

void Response::serveCgi_(const Request &request) {
  CgiConnector cgi(request);
  cgi.makeConnection(this->status_);
  if (status_ > 399) {
    this->status_ >> this->body_;
    this->header_.insert(contentField("Content-Type", "text/html"));
    this->header_.insert(contentField("Connection", "keep-alive"));
    this->header_.insert(
        contentField("Content-Length", toString(this->body_.length())));
    return;
  }
  this->header_ = cgi.getHeader();
  this->body_ = cgi.getBody();
}

/*            global funnctions                  */

contentMap Response::createTypeMap() {
  std::ifstream data("./resources/filetypes.csv");
  std::string field;
  std::string key;
  std::string value;
  contentMap fileTypes;

  while (data.is_open() && std::getline(data, field)) {
    try {
      key = field.substr(0, field.find(","));
      value = field.substr(field.find(",") + 1);
    } catch (std::exception &) {
      continue;
    }
    fileTypes.insert(contentField(key, value));
  }
  return (fileTypes);
}
