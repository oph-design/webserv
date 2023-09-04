#ifndef OS_HPP_
#define OS_HPP_

#define MACOS 0
#define LINUX 1
#define OTHER_OS 2

#if defined(__APPLE__)
#define OS 0
#elif defined(__linux__)
#define OS 1
#else
#define OS 2
#endif



#endif  // OS_HPP_
