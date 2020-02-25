#ifndef NONOVERLAPINITIALIZER_H
#define NONOVERLAPINITIALIZER_H

#include "randominitializer.h"
#include <random>
#include <stdexcept>

class NonoverlapInitializer : public RandomInitializer {
public:
    NonoverlapInitializer(double distance, double _radius, int _seed = 1)
        : RandomInitializer(distance, _seed), radius(_radius){};
  void place(double **particles, size_t, size_t) override;
    const double radius;
};

void NonoverlapInitializer::place(double **particles, size_t size,
                                  size_t dimensions) {
    int overlaping[size];
    size_t counter = 0;
    do{
        RandomInitializer::place(particles, size, dimensions);
        if(counter++ > 1000){
            throw std::runtime_error("Could not place particles nonoverlapping");
        }
    }while(overlaps(particles, size, dimensions, 2.1*radius, overlaping) > 0);
}

#endif /* NONOVERLAPINITIALIZER_H */
