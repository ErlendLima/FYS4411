#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <string>
#include <fstream>
#include <iostream>

template <class T> static std::string dtypeof();
template <> std::string dtypeof<int>() { return "int64"; };
template <> std::string dtypeof<float>() { return "float32"; };
template <> std::string dtypeof<double>() { return "float64"; };

class AObservable {
public:
    AObservable(const std::string &_name, const std::string &filename,
              size_t _size)
        : name(_name), fname(filename), size(_size){};
    virtual ~AObservable(){};

    virtual void dump(const std::string& path) const = 0;
    std::string getType(){return dtype;};

    const std::string name;
    const std::string fname;
    const size_t size;
protected:
    std::string dtype;
};

template <class T>
class Observable: public AObservable
{
public:
    Observable(const std::string& _name,
               const std::string& filename, size_t _size)
        : AObservable(_name, filename, _size){
        values = new T[size];
        dtype = dtypeof<T>();
    };
    ~Observable(){delete values;}
    void dump(const std::string& path) const override{
        std::ofstream file;
        file.open(path + "/" + fname, std::ios::binary);
        file.write((char*)values, size*sizeof(T));
        file.close();
    };
    void store(T val){
        values[i++] = val;
    }
    T last() const{
        return values[i-1];
    }

  private:
    size_t i = 0;
    T* values;
};

#endif /* OBSERVABLE_H */
