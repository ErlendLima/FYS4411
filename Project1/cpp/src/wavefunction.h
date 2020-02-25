// -*- lsst-c++ -*-
#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H

#include <memory>
#include "distribution.h"
#include "initializer.h"

class Wavefunction : public Distribution
{
public:
    Wavefunction();
    virtual ~Wavefunction();

    void initialize(size_t num_particles, size_t num_dimensions, Initializer*);
    double oneBodyElement(int index) const;
    double correlation(int index1, int index2) const;

    double probabilityDensity() const override;
    void changeCandidate(int index, double* new_values) override;
    void restoreCandidate(int index) override;

    void dump(const std::string& path);

    Initializer* initializer;

protected:
    double mass        = 1.0;
    double omega       = 1.0;
    double radius      = 1.0;
    double beta        = 1.0;
    double alpha       = 0.5;
    double step_length = 1.0;

    size_t num_particles  = 0;
    size_t num_dimensions = 0;

    // Dimensions num_particles x num_dimensions
    double **particles = nullptr;
};



#endif /* WAVEFUNCTION_H */
