#include "hamiltonian.h"


double Hamiltonian::localEnergy(Wavefunction& wavefunction) const{
    double Ek = kineticEnergy(wavefunction);
    double Ep = potentialEnergy(wavefunction);
    return Ek + Ep;
}

double Hamiltonian::kineticEnergy(Wavefunction& wavefunction) const{
    double Ek = 0;//(*laplacian)(wavefunction);
    return - hbar/(2*mass) * Ek;
}
