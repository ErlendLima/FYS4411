#include <iostream>
#include "parameters.h"

int main(int argc, char const *argv[]){
    std::string filename = "../data/metatest.toml";
    if (argc >= 2)
      filename = std::string(argv[1]);
    auto params = Parameters(filename);
  return 0;
}
