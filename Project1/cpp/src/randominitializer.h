#ifndef RANDOMINITIALIZER_H
#define RANDOMINITIALIZER_H

#include <random>
#include "initializer.h"
#include "macros.h"

class RandomInitializer: public Initializer {
public:
    RandomInitializer(double distance, int _seed=1): Initializer(distance, _seed){};
    virtual void place(double **particles, size_t, size_t) override;
};

void RandomInitializer::place(double **particles, size_t size, size_t dimensions){
  auto gen = std::mt19937_64(seed);
  auto dist = std::uniform_real_distribution<double>(-1, 1);
  double *position;
  for (size_t i = 0; i < size; ++i) {
    position = particles[i];
    for (size_t j = 0; j < dimensions; j++) {
      position[j] = average_distance* dist(gen);
    }
  }
}

#endif /* RANDOMINITIALIZER_H */
