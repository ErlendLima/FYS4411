// -*- lsst-c++ -*-
#ifndef HAMILTONIAN_H
#define HAMILTONIAN_H

#include "wavefunction.h"
#include "laplacian.h"

class Hamiltonian
{
public:
    Hamiltonian(Laplacian* lap, double m): laplacian(lap), mass(m){};
    virtual ~Hamiltonian(){delete laplacian;};

    /**
       Both kinetic energy and local energy are common for
       all traps. Potential is not, and must be specified.
    */
    virtual double kineticEnergy(Wavefunction &) const;
    double localEnergy(Wavefunction &) const;

    virtual double potentialEnergy(const Wavefunction &) const = 0;
protected:
  Laplacian *laplacian;
  const double mass = 1.0;
  const double hbar = 1.0;
};



#endif /* HAMILTONIAN_H */
