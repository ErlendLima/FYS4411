#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include "observable.h"
#include "cpptoml.h"

class FileManager
{
public:
    FileManager(const std::string& savepath): path(savepath){};
    virtual ~FileManager(){};

    // template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    // void binaryDump(std::ofstream &, const std::vector<T> &) const;

    void dumpAll() const;
    template <class T>
    void add(Observable<T> *observable){
        observables.push_back((AObservable *)observable);
    }
private:
    const std::string path;
    const std::string metafile = "index.toml";
    std::vector<AObservable* > observables;
};


#endif /* FILEMANAGER_H */
