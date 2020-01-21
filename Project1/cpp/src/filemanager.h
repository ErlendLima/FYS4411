#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>

class FileManager
{
public:
    FileManager();
    virtual ~FileManager();

    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void binaryDump(std::ofstream &, const std::vector<T> &) const;
};


#endif /* FILEMANAGER_H */
