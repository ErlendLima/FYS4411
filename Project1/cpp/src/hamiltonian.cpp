#include "hamiltonian.h"


double Hamiltonian::localEnergy(Wavefunction& wavefunction) const{
    const double Ek = kineticEnergy(wavefunction);
    const double Ep = potentialEnergy(wavefunction);
    return Ek + Ep;
}

double Hamiltonian::kineticEnergy(Wavefunction& wavefunction) const{
    const double Ek = (*laplacian)(wavefunction);
    return - SQ(hbar)/(2*mass) * Ek;
}
