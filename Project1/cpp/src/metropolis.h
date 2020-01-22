#ifndef METROPOLIS_H
#define METROPOLIS_H

#include "distribution.h"
#include <random>

class Metropolis
{
public:
    Metropolis(int _seed = -1, int upper_bound=1);
    virtual ~Metropolis(){};

    virtual bool step(Distribution&);

    template<class T>
    T draw();

    template <class T> void draw(T *arr, size_t number) {
      for (size_t i = 0; i < number; ++i) {
        arr[i] = draw<T>();
      }
    }

    double drawTheta(){return theta_distribution(gen);};


  private:
    int seed;
    // The cache is used to store the random numbers to be
    // passed to the awaiting Distribution. Prevents unnecessary allocations.
    double cache[3] = {0.0};
    //std::random_device rd;
    std::mt19937_64 gen;
    std::normal_distribution<double> normal_distribution;
    std::uniform_real_distribution<double> uniform_distribution;
    std::uniform_real_distribution<double> theta_distribution;
    std::uniform_int_distribution<int> uniform_int_distribution;

};


#endif /* METROPOLIS_H */
