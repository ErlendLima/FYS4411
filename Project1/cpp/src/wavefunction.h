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
    Wavefunction(double _alpha, double _beta, double _radius, double _stepsize);
    virtual ~Wavefunction();

    void initialize(size_t num_particles, size_t num_dimensions, std::shared_ptr<Initializer>);
    double oneBodyElement(size_t index) const;
    double correlation(size_t index1, size_t index2) const;
    double correlation(const double*, const double*) const;

    double evaluate() const;
    double probabilityDensity() const override;
    double probabilityRatio() const override;
    void changeCandidate(size_t index, double* new_values) override;
    void restoreCandidate(size_t index) override;
    double oneBodyElementOriginal() const;

    void dump(const std::string& path);

    //Initializer* initializer;
    size_t nparticles() const {return num_particles;};

    double* position(size_t index)const {return particles[index];};
    double const*const* getParticles() const {return particles;};

    const double mass = 1.0;
    const double alpha = 0.5;
    const double beta = 1.0;
    const double radius = 1.0;
    const double step_length = 1.0;


  protected:

    size_t num_particles = 0;

    // Dimensions num_particles x num_dimensions
    double **particles   = nullptr;

    friend class Hamiltonian;
    friend class Laplacian;
};



#endif /* WAVEFUNCTION_H */
