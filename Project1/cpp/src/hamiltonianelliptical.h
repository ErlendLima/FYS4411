#ifndef HAMILTONIANELLIPTICAL_H
#define HAMILTONIANELLIPTICAL_H

#include "hamiltonian.h"

class HamiltonianElliptical: public Hamiltonian
{
public:
    HamiltonianElliptical(Laplacian* lap, double mass, double omega_ho, double omega_z);
    virtual ~HamiltonianElliptical(){};
    double potentialEnergy(const Wavefunction&) const override;
protected:
    const double lambdasq = 1.0;
    const double omegasq = 1.0;
};


#endif /* HAMILTONIANELLIPTICAL_H */
