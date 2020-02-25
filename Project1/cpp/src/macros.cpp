#include "macros.h"

void printlog(const char *fmt, ...) {
  char str[256];
  va_list args;
  va_start(args, fmt);
  vsprintf(str, fmt, args);
  va_end(args);

  printf("%s\n", str);
}
