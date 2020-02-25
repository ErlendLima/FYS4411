#ifndef GRIDINITIALIZER_H
#define GRIDINITIALIZER_H

#include "initializer.h"
#include <iostream>
#include <math.h>
#include "macros.h"

double nsqrt(double x, double n){
    double res = x;
    for(size_t i = 1; i < n; i++)
        res = sqrt(res);
    return res;
}

class GridInitializer: Initializer
{
public:
  GridInitializer(double distance, int _seed = 1)
      : Initializer(distance, _seed){};
  void place(double **particles, size_t, size_t) override;
};

void GridInitializer::place(double **particles, size_t size,
                            size_t dimensions) {
  double *position;
  double particles_per_side = nsqrt(size, dimensions);
  int num_particles[3] = {0};

  LOGD(particles_per_side);

  for (size_t i = 0; i < size; ++i) {
    position = particles[i];
    for (size_t j = 0; j < dimensions; j++) {
        position[j] = average_distance*num_particles[j];
    }

    if (num_particles[0]++ >= particles_per_side) {
        num_particles[0] = 0;
        if (num_particles[1]++ >= particles_per_side){
            num_particles[1] = 0;
            if(num_particles[2]++ >= particles_per_side){
                num_particles[2] = 0;
            }
        }
    }
  }
  LOGD(num_particles[0]);
  //delete position;
}

#endif /* GRIDINITIALIZER_H */
