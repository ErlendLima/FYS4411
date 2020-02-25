#include "simulation.h"
#include "parameters.h"
#include <stdexcept>
#include "initializer.h"
#include "randominitializer.h"
#include "gridinitializer.h"
#include "nonoverlapinitializer.h"


Simulation::Simulation(const std::string& path){
    parameters.read(path);
}

void Simulation::setup(){
    if (!parameters.hasRead())
        throw std::runtime_error("Read parameters before running");
    Initializer* initializer = NULL;
    switch (parameters.placement) {
    case InitialPlacement::Random: {
      initializer  = (Initializer*)  new RandomInitializer(parameters.distance);
      break;
    }
    case InitialPlacement::Grid: {
        initializer = (Initializer*) new GridInitializer(parameters.distance);
        break;
    }
    case InitialPlacement::Nonoverlap: {
        initializer = (Initializer*) new NonoverlapInitializer(parameters.distance, parameters.radius);
        break;
        default: break; // Unreachable
    }
    }
    Wavefunction wavefunction = Wavefunction();
    wavefunction.initialize(parameters.num_particles, parameters.dimensions, initializer);
    wavefunction.dump("../data/position.xyz");
    delete initializer;
}
