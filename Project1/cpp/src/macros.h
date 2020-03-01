#ifndef MACROS_H
#define MACROS_H

#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <sstream>
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

void printlog(const char* source, const char *fmt, ...);
//void printlog(const char *fmt, ...);
void printstamp(const char* source, const std::string&);
#if 0
#define LOG(...) printlog(__FILENAME__, __VA_ARGS__)
#define LOGD(x)                                                                \
  do {                                                                         \
  std::stringstream ss;\
    ss << #x << ": " << x;                                 \
    printstamp(__FILENAME__, ss.str());                                  \
  } while (0)
#else
#define LOG(...)                                                          \
  do {                                                                         \
  } while (0)
#define LOGD(x)                                                                \
  do {                                                                         \
  } while (0)
#endif

#define SQ(x) ((x)*(x))

#include <chrono>
#define BENCHMARK(func, repeats)                                               \
  do {                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                    \
    for (size_t i = 0; i < repeats; i++) {                                      \
      func;                                                                    \
    }                                                                          \
    auto stop = std::chrono::high_resolution_clock::now();                     \
    auto duration =                                                            \
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);   \
    std::cout << "Duration of " << #func << ": " << duration.count()/repeats  << " ms"<< std::endl;         \
  } while (0)

#endif /* MACROS_H */
