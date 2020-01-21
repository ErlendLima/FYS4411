#include "filemanager.h"
#include <fstream>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
void FileManager::binaryDump(std::ofstream &stream,
                             const std::vector<T> &container) const {
  stream.write((char *)&container[0], sizeof(container[0]));
}
