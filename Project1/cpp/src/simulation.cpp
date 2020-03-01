#include "simulation.h"
#include "filemanager.h"
#include "laplacian.h"
#include "parameters.h"
#include <stdexcept>
#include "initializer.h"
#include "randominitializer.h"
#include "gridinitializer.h"
#include "nonoverlapinitializer.h"
#include "hamiltonian.h"
#include "hamiltonianelliptical.h"

Simulation::Simulation(const std::string& path){
    parameters.read(path);
    if (parameters.record_observables){
        auto size = parameters.mc_cycles / parameters.record_period;
        local_energies = new Observable<double>("Local Energy", "local_energies.bin", size);
        filemanager.add(local_energies);
        //onebody_densities = new double[parameters.mc_cycles];
    }
}

Simulation::~Simulation(){
}

void Simulation::memoryUsage() const noexcept{
    auto particles = sizeof(double*) * parameters.num_particles + sizeof(double)*parameters.num_particles*parameters.dimensions;
    auto records = (parameters.record_observables) ? 2*sizeof(double)*parameters.mc_cycles : 0;
    std::cout << "Memory usage: " << particles + records << "\n"
              << "  -Wavefunction: " << particles << "\n"
              << "  -Observables: " << records << std::endl;
}

void Simulation::setup(){
    if (!parameters.hasRead())
        throw std::runtime_error("Read parameters before running");

    // Setup Initializer
    switch (parameters.placement) {
    case InitialPlacement::Random: {
        initializer = std::make_shared<RandomInitializer>(parameters.distance);
      break;
    }
    case InitialPlacement::Grid: {
        initializer = std::make_shared<GridInitializer>(parameters.distance);
        break;
    }
    case InitialPlacement::Nonoverlap: {
        initializer = std::make_shared<NonoverlapInitializer>(parameters.distance, parameters.radius);
        break;
        default: throw std::runtime_error("Unreachable Code");
    }
    }

    // Setup Hamiltonian
    const double omega_ho = 1.0;
    const double omega_z = 1.0;
    Laplacian* laplacian = new LaplacianNumerical();
    hamiltonian = std::make_unique<HamiltonianElliptical>(laplacian, 1.0, omega_ho, omega_z);
    // Setup Metropolis
    metropolis = Metropolis(-1, parameters.num_particles-1);
}

Wavefunction Simulation::makeWavefunction(){
    Wavefunction wavefunction = Wavefunction(parameters.alpha, parameters.beta,
                                             parameters.radius, parameters.stepsize);
  wavefunction.initialize(parameters.num_particles, parameters.dimensions,
                           initializer);
  return wavefunction;
}

void Simulation::run(){
    Wavefunction wavefunction = makeWavefunction();
    wavefunction.dump("../data/initial.xyz");

    size_t accepted = 0;
    for(size_t step = 0; step < parameters.mc_cycles; step++){
        accepted += metropolis.step(wavefunction);

        if (parameters.record_observables && (step % parameters.record_period) == 0){
            local_energies->store(hamiltonian->localEnergy(wavefunction));
            auto last = local_energies->last();
            LOGD(last);
        }
    }
    std::cout << "Acceptance rate: [" << accepted << "]   " << ((double)accepted)/parameters.mc_cycles * 100 << "%\n";
    wavefunction.dump("../data/final.xyz");
}
