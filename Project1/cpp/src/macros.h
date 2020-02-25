#ifndef MACROS_H
#define MACROS_H

#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>

void printlog(const char *fmt, ...);
#ifdef DEBUG
#define LOG printlog
#define LOGD(x)                                                                \
  do {                                                                         \
    std::cerr << #x << ": " << x << std::endl;                                 \
  } while (0)
#else
#define LOG(x)                                                                 \
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
