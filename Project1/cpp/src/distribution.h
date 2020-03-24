#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H


#include <cstddef>
class Distribution
{
public:
    Distribution(){};
    virtual ~Distribution(){};
    virtual double probabilityDensity() const = 0;
    virtual double probabilityRatio() const = 0;
    virtual size_t dimensions() const {return num_dimensions;}

    virtual void changeCandidate(size_t index, double* new_values) = 0;
    virtual void restoreCandidate(size_t index) = 0;

  protected:
    size_t num_dimensions;
    // Cache used to store the old values during the Metropolis step
    // to prevent unnecessary temporary allocations.
    double cache[3] = {0.0};
    size_t index_moved = 0;
};



#endif /* DISTRIBUTION_H */
