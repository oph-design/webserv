void signalHandling() {
#ifdef __linux__
  signal(SIGPIPE, SIG_IGN);
#endif
}
