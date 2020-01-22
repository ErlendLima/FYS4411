#include "metropolis.h"
#include "distribution.h"
#include <iostream>
#include <random>
//#include "macros.h"

#define SQ(x) x*x

class Gauss : public Distribution
{
public:
    Gauss(double mean, double std, int _length=100){
        std::default_random_engine gen;
        std::normal_distribution<double> dist(mean, std);
        length = _length;
        values = new double[length];

        for(size_t i = 0; i < length; i++){
            values[i] = dist(gen);
        }
    };
    virtual ~Gauss(){};
    double* values;

    double probabilityDensity(){
        double S = 0.0;
        for (size_t i = 0; i < length; ++i) {
            S += SQ(values[i]);
        }
        return S/length;
    };

    int dimensions(){return 1;};
    int length;
    void changeCandidate(int index, double* new_values){
        cache[0] = values[index];
        values[index] += new_values[0];
    }
    void restoreCandidate(int index) { values[index] = cache[0]; }
};


int main(int argc, char *argv[])
{
    Metropolis met = Metropolis();
    std::cout << met.draw<double>() << std::endl;
    std::cout << met.draw<int>() << std::endl;
    double arr[3] = {0.0};
    std::cout << arr[0] << ", " << arr[1] << ", " << arr[2] << std::endl;
    met.draw(arr, 3);
    std::cout << arr[0] << ", " << arr[1] << ", " << arr[2] << std::endl;

    int arr2[3] = {0};
    std::cout << arr2[0] << ", " << arr2[1] << ", " << arr2[2] << std::endl;
    met.draw(arr2, 3);
    std::cout << arr2[0] << ", " << arr2[1] << ", " << arr2[2] << std::endl;

    Gauss g = Gauss(0.0, 1.0, 3);
    Metropolis M = Metropolis(-1, 3);
    int j = 0;
    std::cout << g.probabilityDensity() << std::endl;
    for (size_t i = 0; i < 10; ++i) {
        if(M.step(g)){
            std::cout << g.cache[0] << std::endl;
          std::cout << g.values[0] << ", " << g.values[1] << ", " << g.values[2]
                    << std::endl;
          j++;
        }
    }
    std::cout << g.probabilityDensity() << std::endl;
    std::cout << j << std::endl;

    return 0;
}
