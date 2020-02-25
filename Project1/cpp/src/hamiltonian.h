// -*- lsst-c++ -*-
#ifndef HAMILTONIAN_H
#define HAMILTONIAN_H

#include "wavefunction.h"


class Hamiltonian
{
public:
    Hamiltonian(){};
    virtual ~Hamiltonian();

    /**
       Both kinetic energy and local energy are common for
       all traps. Potential is not, and must be specified.
    */
    virtual double kinetic_energy(const Wavefunction &) const;
    double local_energy(const Wavefunction &) const;

    virtual double potential_energy(const Wavefunction &) const = 0;
};



#endif /* HAMILTONIAN_H */
