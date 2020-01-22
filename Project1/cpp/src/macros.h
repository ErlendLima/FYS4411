#ifdef DEBUG
#define LOG print
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

#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>

void print(const char *fmt, ...) {
  char str[256];
  va_list args;
  va_start(args, fmt);
  vsprintf(str, fmt, args);
  va_end(args);

  printf("%s\n", str);
}

#define SQ(x) x *x
