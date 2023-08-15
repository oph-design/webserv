#include "CgiConnector.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request& request)
    : isCgi(checkCgi()),
      reqBody_(request.getRequestBody()),
      env_(buildEnv(request)) {}

CgiConnector::CgiConnector(const CgiConnector& rhs) { *this = rhs; }

CgiConnector& CgiConnector::operator=(const CgiConnector& rhs) {
  this->respBody_ = rhs.respBody_;
  this->respHeader_ = rhs.respHeader_;
  this->env_ = rhs.env_;
  return (*this);
}

CgiConnector::~CgiConnector() { this->env_.clear(); }

bool CgiConnector::checkCgi() {
  std::string name = this->env_["SCRIPT_NAME"];
  std::string extention = name.substr(name.rfind(".") + 1, name.length());
  if (extention != "php" && extention != "py" && extention != "pl")
    return (false);

  struct dirent* file;
  DIR* bin = opendir("./cgi-bin");
  while ((file = readdir(bin)) != NULL)
    if (name == file->d_name) return (closedir(bin), true);
  closedir(bin);
  return (false);
}

envMap CgiConnector::buildEnv(const Request& request) {
  envMap res;
  std::string uri = request.getPath();
  res.insert(envVar("REQUEST_METHOD", request.getRequestMethodString()));
  try {
    res.insert(envVar("CONTENT_TYPE", request["Content-Type"]));
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  try {
    res.insert(envVar("CONTENT_LENGTH", request["Content-Length"]));
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  res.insert(envVar("QUERY_STRING", request.getQueryString()));
  res.insert(envVar("REQUEST_URI", uri));
  res.insert(
      envVar("SCRIPT_NAME", uri.substr(uri.rfind("/") + 1, uri.length())));
  res.insert(envVar("SERVER_PROTOCOL", request.getHTTPVersion()));
  return (res);
}

char** CgiConnector::envToString() {
  size_t i = 0;
  std::string variable;
  char** res = new char*[this->env_.size() + 1];
  for (envMap::iterator it = env_.begin(); it != env_.end(); ++it) {
    variable = it->first + "=" + it->second;
    res[i] = new char[variable.length() + 1];
    strcpy(res[i], variable.c_str());
    res[i][variable.length()] = 0;
    ++i;
  }
  res[i] = NULL;
  return (res);
}

void CgiConnector::handlePipes(t_fds& fdStruct) {
  fdStruct.stdIn = dup(0);
  fdStruct.stdOut = dup(1);
  pipe(fdStruct.pipes);
  dup2(fdStruct.pipes[1], 1);
  dup2(fdStruct.pipes[0], 0);
  close(fdStruct.pipes[1]);
  close(fdStruct.pipes[0]);
}

void CgiConnector::handlePipes(t_fds& fdStruct, int) {
  dup2(fdStruct.stdOut, 1);
  dup2(fdStruct.stdIn, 0);
  close(fdStruct.stdIn);
  close(fdStruct.stdOut);
}

void CgiConnector::readOutput() {
  char buffer[1];
  std::string str;
  while (read(1, buffer, 1)) str.append(buffer);
  this->respHeader_ = str.substr(0, str.find("\r\n\r\n") + 4);
  this->respBody_ = str.substr(str.find("\r\n\r\n") + 4, str.length());
}

int CgiConnector::makeConnection() {
  t_fds fdStruct;
  char** env = envToString();
  char* root = getenv("PWD");
  std::string path = std::string(root) + this->env_["SCRIPT_NAME"];
  delete root;
  this->handlePipes(fdStruct);
  if (fork() == 0) {
    write(0, reqBody_.c_str(), reqBody_.length());
    execve(path.c_str(), NULL, env);
    return (500);
  }
  this->readOutput();
  this->handlePipes(fdStruct, 1);
  return (200);
}
