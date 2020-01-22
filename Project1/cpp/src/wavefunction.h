// -*- lsst-c++ -*-
#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H

#include <memory>
#include "distribution.h"

class Wavefunction : public Distribution
{
public:
    Wavefunction();
    virtual ~Wavefunction();

    void initialize(size_t num_particles, size_t num_dimensions);
    void initializeGrid(double spread);
    double oneBodyElement(int index) const;
    double correlation(int index1, int index2) const;

    double probabilityDensity() const override;
    void changeCandidate(int index, double* new_values) override;
    void restoreCandidate(int index) override;

protected:
    double mass        = 1.0;
    double omega       = 1.0;
    double radius      = 1.0;
    double beta        = 1.0;
    double alpha       = 1.0;
    double step_length = 1.0;
    double grid_spread = 1.0;
    int grid_seed      = 1;

    size_t num_particles;
    size_t num_dimensions;

    // Will be a two dimensional array
    // of size num_particles, num_dimensions
    double **particles;

    double cache[3] = {0.0};

};



#endif /* WAVEFUNCTION_H */
