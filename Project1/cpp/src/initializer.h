#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <cstddef>
#include "macros.h"

static inline double distanceSQ(const double *r1, const double *r2,
                                size_t dimensions) {
  double distanceSQ = 0.0;
  for (size_t i = 0; i < dimensions; ++i)
    distanceSQ += SQ(r1[i] - r2[i]);
  return distanceSQ;
}

class Initializer {
  public:
    Initializer(double distance, int _seed=1): seed(_seed), average_distance(distance){};
    virtual ~Initializer(){};
    virtual void place(double **particles, size_t, size_t) = 0;

    const unsigned int seed = 1;
    const double average_distance;

    bool overlap(double **const particles, size_t size, size_t dimensions,
                 double distance, double *particle) {
      double sqdistance = SQ(distance);
      for (size_t i = 0; i < size; i++) {
        if (particle == particles[i])
          continue;
        if (distanceSQ(particle, particles[i], dimensions) >= sqdistance)
          return true;
      }
      return false;
    }

    size_t overlaps(double **const particles, size_t size, size_t dimensions,
                    double distance, int *overlapping) {
      size_t k = 0;
      double sqdistance = SQ(distance);
      const double *particle;
      for (size_t i = 0; i < size; i++) {
        particle = particles[i];
        for (size_t j = i + 1; j < size; j++) {
          if (distanceSQ(particle, particles[j], dimensions) <= sqdistance) {
              overlapping[k++ - 1] = i;
              break;
          }
        }
      }
      return k;
    }
};



#endif /* INITIALIZER_H */
