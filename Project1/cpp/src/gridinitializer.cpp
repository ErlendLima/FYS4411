#include "gridinitializer.h"

static inline size_t nsqrt(size_t x, size_t n) {
  size_t res = x;
  for (size_t i = 1; i < n; i++)
    res = sqrt(res);
  return res;
}

void GridInitializer::place(double **particles, size_t size,
                            size_t dimensions) {
  double *position;
  size_t particles_per_side = nsqrt(size, dimensions)-1;
  size_t num_particles[3] = {0};

  LOGD(particles_per_side);

  for (size_t i = 0; i < size; ++i) {
    position = particles[i];
    for (size_t j = 0; j < dimensions; j++) {
        position[j] = average_distance * num_particles[j];
    }

    if (num_particles[0]++ >= particles_per_side) {
        num_particles[0] = 0;
        if (num_particles[1]++ >= particles_per_side) {
            num_particles[1] = 0;
            if (num_particles[2]++ >= particles_per_side) {
                num_particles[2] = 0;
            }
        }
    }
  }
  LOGD(num_particles[0]);
}

