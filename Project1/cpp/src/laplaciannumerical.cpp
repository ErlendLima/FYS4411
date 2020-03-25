#include "laplacian.h"

/**
   Compute the laplacian ∇² of a wavefunction
 */
double LaplacianNumerical::operator()(Wavefunction& wf){
    double forward, backward, middle, sum;
    double* position;

    middle = wf.evaluate();
    sum = 0;
    for(size_t i = 0; i < wf.nparticles(); i++){
        position = wf.position(i);
        for(size_t j = 0; j < wf.dimensions(); j++){
            position[j] += h;
            forward = wf.evaluate();
            position[j] -= 2*h;
            backward = wf.evaluate();
            position[j] += h;
            sum += (forward - 2*middle + backward)/(h*h);
        }
    }
    // -= 2*d*N*middle ?
    //sum /= h*h*middle;
    //evaluate
    return sum/middle;
}
