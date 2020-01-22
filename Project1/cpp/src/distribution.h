#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H


class Distribution
{
public:
    Distribution(){};
    virtual ~Distribution(){};
    virtual double probabilityDensity() const = 0;
    virtual size_t dimensions() const {return num_dimensions;}

    virtual void changeCandidate(int index, double* new_values) = 0;
    virtual void restoreCandidate(int index) = 0;


  protected:
    size_t num_dimensions;
    // Cache used to store the old values during the Metropolis step
    // to prevent unnecessary temporary allocations.
    double cache[3] = {0.0};
};



#endif /* DISTRIBUTION_H */
