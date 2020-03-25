#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <initializer_list>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>

template <class T> static std::string dtypeof();
template <> std::string dtypeof<int>() { return "int64"; };
template <> std::string dtypeof<float>() { return "float32"; };
template <> std::string dtypeof<double>() { return "float64"; };

class AObservable {
public:
    AObservable(const std::string& _name, const std::string &filename,
                std::initializer_list<size_t> sizes):name(_name), fname(filename)

        {
            size = 1;
            for(auto& s: sizes){
                shape.push_back(s);
                size *= s;
            }
        };
    virtual ~AObservable(){};

    virtual void dump(const std::string& path) const = 0;
    std::string getType(){return dtype;};
    std::vector<size_t> getShape(){return shape;};
    size_t getSize(){return size;};

    const std::string name;
    const std::string fname;
protected:
    std::string dtype;
    std::vector<size_t> shape;
    size_t size;
};

template <class T>
class Observable: public AObservable
{
public:
    Observable(const std::string& _name,
               const std::string& filename,
               std::initializer_list<size_t> sizes)
        : AObservable(_name, filename, sizes){
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
    void push_back(T val){
        if(i >= size)
            reallocate();
        store(val);
    }

    void push_back(const T* ptr, size_t length){
        // Sloppy resizing
        if(i + length >= size)
            reallocate(length);
        for(size_t j = 0; j < length; j++)
            values[i++] = ptr[j];
    }

    void push_back(T const* const* ptr, size_t rows, size_t columns){
        if(i + rows*columns >= size)
            reallocate(rows*columns);
        for(size_t j = 0; j < rows; j++){
            for(size_t k = 0; k < columns; k++)
              values[i++] = ptr[j][k];
        }
    }

    void reallocate(size_t increment = 4096){
        T* temp = new T[size + increment];
        std::copy(values, values+i, temp);
        delete[] values;
        values = temp;
    }

    void shrink(){
        // Pretend the size is [size, num_cols]
        size = i;
        auto tailsize = std::accumulate(shape.begin()+1, shape.end(), 1l, std::multiplies<size_t>());
        shape.front() = (size) / tailsize;
    }

  private:
    size_t i = 0;
    T* values;
};

#endif /* OBSERVABLE_H */
