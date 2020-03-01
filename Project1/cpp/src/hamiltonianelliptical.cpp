#include "hamiltonianelliptical.h"

HamiltonianElliptical::HamiltonianElliptical(Laplacian* lap, double m, double omega_ho, double omega_z)
    : Hamiltonian(lap, m), lambdasq(SQ(omega_z/omega_ho)), omegasq(SQ(omega_ho))
{}

/**
   Compute the potential energy of an elliptical trap
           V(r) = ½mω²(x² + y² + λ²z²)
                = ½m[ω²(x² + y²) + ωz²z²]
 */
double HamiltonianElliptical::potentialEnergy(const Wavefunction& wf) const{
    /* The V_int term is not computed to save time. The case for
       |r_i - r_j| < a is already dealt with in the metropolis step
       TODO: Eliminate branching and measure effect on performance
    */
    double sum = 0.0;
    const size_t ndim = wf.dimensions();
    double* position;
    for(size_t i = 0; i < wf.nparticles(); i++){
        position = wf.position(i);
        sum += SQ(position[0]);
        if (ndim > 1){
            sum += SQ(position[1]);
            if (ndim > 2){
                sum += lambdasq*SQ(position[2]);
            }
        }
    }
    return 0.5*mass*omegasq*sum;
}
