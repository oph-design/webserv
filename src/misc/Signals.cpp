#include "Signals.hpp"

static void signalHandlerSigint(int signum);

bool serverRunning = true;

void implementSignalHandling() {
  signal(SIGINT, signalHandlerSigint);
#ifdef __linux__
  signal(SIGPIPE, SIG_IGN);
#endif
}

static void signalHandlerSigint(int signum) {
  if (signum != SIGINT) return;
  serverRunning = false;
  std::cout << "Server now stopping\nPress Ctrl+C again to force stop"
            << std::endl;
  signal(SIGINT, SIG_DFL);
}
