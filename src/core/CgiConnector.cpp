#include "CgiConnector.hpp"

#include "Utils.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request &request, const std::string &path)
    : reqBody_(request.getRequestBody()), env_(buildEnv_(request)) {
  printVerbose(YELLOW, path);
  this->path_ = path;
}

CgiConnector::CgiConnector(const CgiConnector &rhs) { *this = rhs; }

CgiConnector &CgiConnector::operator=(const CgiConnector &rhs) {
  this->respBody_ = rhs.respBody_;
  this->respHeader_ = rhs.respHeader_;
  this->env_ = rhs.env_;
  return (*this);
}

CgiConnector::~CgiConnector() { this->env_.clear(); }

std::map<std::string, std::string> CgiConnector::getHeader() const {
  std::map<std::string, std::string> res;
  std::istringstream iss(this->respHeader_);
  std::string buffer;
  std::string key;
  std::string value;

  while (std::getline(iss, buffer)) {
    try {
      key = buffer.substr(0, buffer.find(':'));
      value = buffer.substr(buffer.find(':') + 2, buffer.length());
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      continue;
    }
    res.insert(std::pair<std::string, std::string>(key, value));
  }
  return (res);
}

std::string CgiConnector::getBody() const { return this->respBody_; }

bool CgiConnector::isCgi(const std::string &path) {
  std::string name = path.substr(path.rfind('/') + 1, path.size());
  std::string extension = name.substr(name.rfind('.') + 1, name.length());
  if (extension != "py" && extension != "sh") return (false);
  if (access(path.c_str(), F_OK | X_OK) == -1) return (false);
  return (true);
}

envMap CgiConnector::buildEnv_(const Request &request) {
  envMap res;
  std::string uri = request.getPath();
  res.insert(envVar("REQUEST_METHOD", request.getRequestMethodString()));
  res.insert(envVar("QUERY_STRING", request.getQueryString()));
  res.insert(envVar("REQUEST_URI", uri));
  res.insert(envVar("SCRIPT_NAME", uri.substr(uri.rfind('/') + 1, uri.size())));
  res.insert(envVar("SERVER_PROTOCOL", request.getHTTPVersion()));
  try {
    res.insert(envVar("CONTENT_TYPE", request["Content-Type"]));
  } catch (std::exception &e) {
    printVerbose(RED, e.what());
  }
  try {
    res.insert(envVar("CONTENT_LENGTH", request["Content-Length"]));
  } catch (std::exception &e) {
    printVerbose(RED, e.what());
  }
  return (res);
}

char **CgiConnector::envToString_() {
  size_t i = 0;
  std::string variable;
  char **res = new char *[this->env_.size() + 1];
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

bool CgiConnector::waitTimeouted(pid_t pid, int *exitcode) {
  pid_t timer = fork();
  pid_t tmp = 0;
  bool res = true;
  if (timer == 0) {
    sleep(TIMEOUT);
    std::exit(112);
  }
  while (tmp == 0) tmp = waitpid(WAIT_ANY, exitcode, WNOHANG);
  if (tmp == timer) {
    kill(pid, SIGTERM);
    res = false;
  } else {
    kill(timer, SIGTERM);
  }
  waitpid(WAIT_ANY, NULL, 0);
  return (res);
}

void CgiConnector::readOutput_(int pipes[2]) {
  close(pipes[1]);
  char buf[16];
  bzero(buf, 16);
  std::string bufferStr;
  while (read(pipes[0], buf, 15) > 0) {
    bufferStr.append(buf);
    bzero(buf, 16);
  }
  this->respHeader_ = bufferStr.substr(0, bufferStr.find("\n\n"));
  this->respBody_ =
      bufferStr.substr(bufferStr.find("\n\n") + 2, bufferStr.size());
  close(pipes[0]);
}

void CgiConnector::writeReqBody() {
  int script_input[2];
  pipe(script_input);
  dup2(script_input[0], 0);
  close(script_input[0]);
  while (write(script_input[1], this->reqBody_.c_str(), 16))
    ;
  close(script_input[1]);
}

std::string getExtension(std::string path) {
  std::string ext = path.substr(path.rfind('.') + 1);
  if (ext == "py")
    return ("/usr/bin/python3");
  else
    return ("/bin/sh");
}

void CgiConnector::executeScript_(const std::string &path, int pipes[2]) {
  if (this->env_["REQUEST_METHOD"] == "POST") this->writeReqBody();
  dup2(pipes[1], 1);
  close(pipes[0]);
  close(pipes[1]);
  char **env = this->envToString_();
  char **args = new char *[3];
  args[0] = const_cast<char *>(getExtension(path).c_str());
  args[1] = const_cast<char *>(path.c_str());
  args[2] = NULL;
  execve(args[0], args, env);
  size_t i = 0;
  while (env[i]) delete env[i++];
  delete[] env;
  delete[] args;
  std::exit(1);
}

void CgiConnector::makeConnection(Status &status) {
  int pipes[2];
  int exitcode;
  pipe(pipes);
  pid_t pid = fork();
  if (!pid) this->executeScript_(this->path_, pipes);
  if (!waitTimeouted(pid, &exitcode)) std::cerr << "CGI Timeout" << std::endl;
  exitcode = WEXITSTATUS(exitcode);
  if (exitcode > 0) {
    close(pipes[0]);
    close(pipes[1]);
    status = 500;
    return;
  }
  this->readOutput_(pipes);
}
