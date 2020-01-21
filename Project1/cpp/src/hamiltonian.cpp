#include "hamiltonian.h"


double Hamiltonian::local_energy(const Wavefunction& wavefunction) const{
    double Ek = kinetic_energy(wavefunction);
    double Ep = potential_energy(wavefunction);
    return Ek + Ep;
}

double Hamiltonian::kinetic_energy(const Wavefunction& wavefunction) const{
    
    double Ek = wavefunction.laplacian();
    return - ħ * Ek * wavefunction.omega();
}
