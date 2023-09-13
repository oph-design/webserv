#include "Response.hpp"

#include "Utils.hpp"

contentMap Response::fileTypes_ = Response::createTypeMap();

/*            constructors                  */

Response::Response(Request &request, const Location &location)
    : location_(location) {
  if (this->redirect()) return;
  switch (request.getRequestMethodType()) {
    case 0:
      handleGetRequest_(request, request.cutPath(location_.getPath()));
      break;
    case 1:
      handlePostRequest_(request, request.cutPath(location_.getPath()));
      break;
    case 2:
      handleDeleteRequest_(request, request.cutPath(location_.getPath()));
      break;
    default:
      this->status_ = 501;
      this->status_ >> this->body_;
  }
}

Response::Response(const Response &rhs) : location_(rhs.location_) {
  *this = rhs;
}

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

std::string Response::getHeader() const {
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

void Response::findType_(const std::string &url) {
  std::string extension;
  contentMap::iterator search;

  extension = url.substr(url.rfind('.') + 1, url.length());
  search = Response::fileTypes_.find(extension);
  if (search != Response::fileTypes_.end()) {
    this->type_ = Response::fileTypes_[extension] + "; charset=UTF-8";
  } else {
    this->type_ = "application/octet-stream; charset=UTF-8";
  }
}

bool Response::isForbiddenPath_(const std::string &dir) {
  std::stringstream ss(dir);
  int counter = 0;
  std::string buffer;
  while (std::getline(ss, buffer, '/')) {
    if (buffer.empty()) continue;
    (buffer == "..") ? ++counter : --counter;
    if (counter > 0) return true;
  }
  return false;
}

void Response::readBody_(const std::string &dir) {
  std::ifstream file(dir.c_str());
  if (file.is_open()) {
    printVerbose("File opened successfully.", "");
  } else {
    printVerbose("Failed to open file.", "");
    this->status_ = 404;
  }
  std::stringstream content;
  content << file.rdbuf();
  file.close();
  this->body_ = content.str();
}

void Response::handleGetRequest_(Request &request, const std::string &uri) {
  std::cout << "in handel reuest: " << request.getPath() << std::endl;
  std::string path = location_.getRoot() + uri;
  if (!this->prerequisites_("GET", request)) return;
  if (location_.getCgiPr() && CgiConnector::isCgi(location_.getCgiPass() + uri))
    return (void)(serveCgi_(request));
  if (Response::isFolder_(path) && !this->location_.getAutoindex())
    path = path + this->location_.getIndex();
  else if (Response::isFolder_(path) && this->location_.getAutoindex())
    return (void)(serveFolder_(path, request));

  if (this->status_ == 200) readBody_(path);
  if (this->status_ == 200) findType_(path);
  if (this->status_ > 399) {
    this->status_ >> this->body_;
    this->type_ = "text/html; charset=UTF-8";
  }
  printVerbose(BLUE, this->status_);
  std::string length = toString<std::size_t>(this->body_.length());

  this->header_.insert(contentField("Content-Type", this->type_));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", length));
}

/*             Post Request                  */

void Response::createFile_(const std::string &filename,
                           const std::string &data) {
  std::string path = this->location_.getUploadPass();
  path = path + "/" + filename;
  this->status_ = 201;
  if (!access(path.c_str(), F_OK)) this->status_ = 200;
  std::ofstream outfile((path).c_str());
  if (!outfile.is_open()) this->status_ = 403;
  outfile.write(data.c_str(), data.length());
  outfile.close();
}

void Response::buildJsonBody_() {
  this->body_ = "{\n";
  this->body_.append("\"status\": ");
  if (this->status_ < 400)
    this->body_.append("\"success\",\n");
  else
    this->body_.append("\"error\",\n");
  this->body_.append("}");
  this->header_.insert(
      contentField("Content-Length", toString(this->body_.size())));
}

std::string Response::getFilename(const Request &request) {
  std::string file = request.getPath();
  std::string ext;
  std::string clen;
  file = file.substr(file.rfind('/') + 1);
  file = file.substr(0, file.rfind('.'));
  try {
    ext = swapColumns(fileTypes_)[request["Content-Type"]];
    clen = request["Content-Length"];
    file = getContentDisposition(request, "filename");
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  if (ext.empty() || clen.empty() || file.empty() ||
      request.getRequestBody().empty())
    this->status_ = 400;
  return (file + "." + ext);
}

void Response::handlePostRequest_(const Request &request,
                                  const std::string &uri) {
  if (!this->prerequisites_("POST", request)) return;
  if (location_.getCgiPr() && CgiConnector::isCgi(location_.getCgiPass() + uri))
    return (void)(serveCgi_(request));
  std::string file = this->getFilename(request);
  if (this->status_ < 400) this->createFile_(file, request.getRequestBody());
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Type", "application/json"));
  buildJsonBody_();
}

/*             Delete Request                  */

void Response::handleDeleteRequest_(const Request &request,
                                    const std::string &uri) {
  std::string path = location_.getRoot() + uri;
  if (!this->prerequisites_("DELETE", request)) return;
  if (location_.getCgiPr() && CgiConnector::isCgi(location_.getCgiPass() + uri))
    return (void)(serveCgi_(request));
  if (access(path.c_str(), F_OK)) this->status_ = 403;
  if (this->status_ < 400 && remove(path.c_str())) this->status_ = 500;
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Type", "application/json"));
  buildJsonBody_();
}

bool Response::prerequisites_(const std::string &method,
                              const Request &request) {
  if (isForbiddenPath_(request.getPath()))
    this->status_ = 400;
  else if (!this->location_.methodAllowed(method))
    this->status_ = 405;
  else if (this->location_.maxBodyReached(request.getRequestBody().size()))
    this->status_ = 413;
  if (this->status_ < 400) return (true);
  this->status_ >> this->body_;
  std::string length = toString<std::size_t>(this->body_.length());
  this->type_ = "text/html; charset=UTF-8";
  this->header_.insert(contentField("Content-Type", this->type_));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", length));
  return (false);
}
/*             Cgi Request                  */

void Response::serveCgi_(const Request &request) {
  CgiConnector cgi(
      request, location_.getCgiPass() + request.cutPath(location_.getPath()));
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

/*                Folder Request                  */
void Response::serveFolder_(const std::string &path, const Request &request) {
  this->body_ = createFolderBody_(path, request);
  this->header_.insert(contentField("Content-Type", "text/html"));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(
      contentField("Content-Length", toString(this->body_.length())));
}

std::deque<std::string> Response::getFilesInFolder_(const std::string &path) {
  std::string folderPath = path.substr(path.rfind('/') + 1, path.size());
  DIR *dir = opendir(path.c_str());
  std::deque<std::string> names;
  if (dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      std::string fileName = entry->d_name;
      if (fileName == "." || (folderPath == "/" && fileName == "..")) continue;
      struct stat fileStat = {};
      if (folderPath[folderPath.length() - 1] != '/') fileName = '/' + fileName;
      const std::string &filePath(path);
      if (stat(filePath.c_str(), &fileStat) == 0) names.push_back(fileName);
    }
    closedir(dir);
  }
  return names;
}

std::string Response::createFolderBody_(const std::string &path,
                                        const Request &request) {
  std::string body;
  std::deque<std::string> names = Response::getFilesInFolder_(path);
  body.append("<html>\n");
  body.append("<head><title>Index of" + request.getPath() +
              " </title></head>\n");
  body.append("<body>\n");
  body.append("<h1>Index of " + request.getPath() + " </h1><hr><pre>\n");
  for (std::deque<std::string>::iterator iter = names.begin();
       iter != names.end(); ++iter) {
    body.append("<a href=\"" + request.getPath() + *iter + "\">");
    if ((*iter)[0] == '/')
      body.append(iter->substr(1));
    else
      body.append(*iter);
    body.append("</a>\n");
  }
  body.append("</pre><hr></body>\n");
  body.append("<html>\n");

  return body;
}

bool Response::isFolder_(const std::string &uri) {
  struct stat st = {};
  if (stat(uri.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
    return true;
  }
  return false;
}

bool Response::redirect() {
  if (this->location_.getRewrite().empty()) return (false);
  this->status_ = 301;
  this->header_.insert(contentField("Location", this->location_.getRewrite()));
  this->header_.insert(contentField("Connection", "Keep-alive"));
  this->body_ = "";
  return (true);
}

/*            global functions                  */

contentMap Response::createTypeMap() {
  std::ifstream data("./data/filetypes.csv");
  std::string field;
  std::string key;
  std::string value;
  contentMap fileTypes;

  while (data.is_open() && std::getline(data, field)) {
    try {
      key = field.substr(0, field.find(','));
      value = field.substr(field.find(',') + 1);
    } catch (std::exception &) {
      continue;
    }
    fileTypes.insert(contentField(key, value));
  }
  return (fileTypes);
}
