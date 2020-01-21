#include <iostream>
#include "cpptoml.h"

int main(int argc, char const *argv[]){
  try {
    std::shared_ptr<cpptoml::table> g = cpptoml::parse_file(argv[1]);
    std::cout << (*g) << std::endl;
  } catch (const cpptoml::parse_exception &e) {
    std::cerr << "Failed to parse " << argv[1] << ": " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
