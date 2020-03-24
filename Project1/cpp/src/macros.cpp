#include "macros.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string.h>


//void printlog(const char *fmt, ...) {
void printlog(const char *source, const char *fmt, ...) {
    char str[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);
    printstamp(source, str);
    // printf("%s\n", str);
  }

void printstamp(const char* source, const std::string& string){
  auto now = std::chrono::system_clock::now();
  std::time_t tnow = std::chrono::system_clock::to_time_t(now);
  char buff[20];
  std::strftime(buff, sizeof(buff), "%T", std::localtime(&tnow));
  const std::chrono::duration<double> tse = now.time_since_epoch();
  std::chrono::seconds::rep milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;

  std::cerr << source << "\t" << buff << "." << milliseconds << "  "<< string << "\n";
}

