#ifndef GRIDINITIALIZER_H
#define GRIDINITIALIZER_H

#include "initializer.h"
#include "macros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>

class GridInitializer: public Initializer
{
public:
  GridInitializer(double distance, int _seed = 1)
      : Initializer(distance, _seed){};
    void place(double **particles, size_t, size_t) override;
};


#endif /* GRIDINITIALIZER_H */
