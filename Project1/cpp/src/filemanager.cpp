#include "filemanager.h"
#include "cpptoml.h"
#include <fstream>
#include <iostream>
#include <type_traits>
#include <vector>

// template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
// void FileManager::binaryDump(std::ofstream &stream,
//                              const std::vector<T> &container) const {
//   stream.write((char *)&container[0], sizeof(container[0]));
// }

void FileManager::dumpAll() const{
    // Create the index file
    auto indexpath = path + "/" + metafile;
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();

    for(auto observable : observables){
        observable->dump(path);
        auto table = cpptoml::make_table();
        table->insert("shape", observable->size);
        table->insert("path", observable->fname);
        table->insert("type", observable->getType());
        root->insert(observable->name, table);
    }

    std::ofstream indexfile;
    indexfile.open(indexpath);
    indexfile << *root;
    indexfile.close();
}
