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
#include "progressmeter.h"

Simulation::Simulation(const std::string& path){
    parameters.read(path);
}

Simulation::~Simulation(){
}

void Simulation::memoryUsage() const noexcept{
    // TODO Plot position as function of time to see development
    auto particles = sizeof(double*) * parameters.num_particles + sizeof(double)*parameters.num_particles*parameters.dimensions;
    auto records = (parameters.record_observables) ? 2*sizeof(double)*parameters.mc_cycles : 0;
    std::cout << "Memory usage: " << particles + records << "\n"
              << "  -Wavefunction: " << particles << "\n"
              << "  -Observables: " << records << std::endl;
}

void Simulation::resetObservables(){
    // if (parameters.record_observables){
    //     auto size = parameters.mc_cycles / parameters.record_period;
    //     //local_energies = new Observable<double>("Local Energy", "local_energies.bin", size);
    //     //kinetic_energy = new Observable<double>("Kinetic Energy", "kinetic_energies.bin", size);
    //     potential_energy = new Observable<double>("Potential Energy", "potential_energies.bin", size);
    //     //filemanager.add(local_energies);
    //     filemanager.add(kinetic_energy);
    //     filemanager.add(potential_energy);
    //     //onebody_densities = new double[parameters.mc_cycles];
    // }
}

void Simulation::setup(){
    if (!parameters.hasRead())
        throw std::runtime_error("Read parameters before running");

    // Setup Initializer
    switch (parameters.placement) {
    case InitialPlacement::Random: {
        std::cout << "Using random placement" << std::endl;
        initializer = std::make_shared<RandomInitializer>(parameters.distance);
      break;
    }
    case InitialPlacement::Grid: {
        std::cout << "Using grid placement" << std::endl;
        initializer = std::make_shared<GridInitializer>(parameters.distance);
        break;
    }
    case InitialPlacement::Nonoverlap: {
        std::cout << "Using no-overlapping placement" << std::endl;
        initializer = std::make_shared<NonoverlapInitializer>(parameters.distance, parameters.radius);
        break;
        default: throw std::runtime_error("Unreachable Code");
    }
    }

    // Setup Hamiltonian
    Laplacian* laplacian;
    if(parameters.laplacian_numerical){
        std::cout << "Using numerical laplacian" << std::endl;
        laplacian = new LaplacianNumerical();
    }
    else if(parameters.radius <= 0){
        std::cout << "Using analytical, non interacting laplacian" << std::endl;
        laplacian = new LaplacianAnalyticalNonInteracting();
    } else {}
        //laplacian = new LaplacianAnalytical();
    hamiltonian = std::make_unique<HamiltonianElliptical>(laplacian, 1.0, parameters.omega_ho, parameters.omega_z);
    // Setup Metropolis
    metropolis = Metropolis(-1, parameters.num_particles-1);
}

Wavefunction* Simulation::makeWavefunction(double alpha){
    Wavefunction* wf =  new Wavefunction(alpha, parameters.beta,
                                         parameters.radius, parameters.stepsize);
    wf->initialize(parameters.num_particles, parameters.dimensions,
                   initializer);
    return wf;
}

void Simulation::setWavefunction(Wavefunction* wf){
    if (wavefunction != nullptr)
        delete wavefunction;
    wavefunction = wf;
}

void Simulation::run(double alpha){
    setWavefunction(makeWavefunction(alpha));
    //wavefunction.dump("../data/initial.xyz");
    ProgressMeter meter(parameters.mc_cycles);
    if (parameters.verbose)
        meter.init();

    size_t accepted = 0;
    double total_Ek = 0;
    double total_Ep = 0;
    for(size_t step = 0; step < parameters.mc_cycles; step++){
        accepted += metropolis.step(*wavefunction);

        total_Ek += hamiltonian->kineticEnergy(*wavefunction);
        total_Ep += hamiltonian->potentialEnergy(*wavefunction);
        //if (parameters.record_observables && (step % parameters.record_period) == 0){
        //    kinetic_energy->store(hamiltonian->kineticEnergy(*wavefunction));
        //    potential_energy->store(hamiltonian->potentialEnergy(*wavefunction));
        //     LOGD(kinetic_energy->last());
            //local_energies->store(hamiltonian->localEnergy(wavefunction));
            //auto last = local_energies->last();
            //LOGD(last);
        // }
        if (parameters.verbose)
            meter.step();
    }
    if (parameters.verbose)
        std::cout << "Acceptance rate: [" << accepted << "]   " << ((double)accepted)/parameters.mc_cycles * 100 << "%\n";
    mean_Ek = total_Ek/parameters.mc_cycles;
    mean_Ep = total_Ep/parameters.mc_cycles;
    //wavefunction.dump("../data/final.xyz");
}

void Simulation::run(AlphaSearch* alphas){
    // Need to refresh observables
    energies = new Observable<double>("Energies", "energies.bin", {100, 3});
    position = new Observable<double>("Position", "position.bin",
                                      {100, parameters.num_particles, parameters.dimensions});
    filemanager.add(energies);
    filemanager.add(position);
    while(alphas->isgood()){
        run(alphas->current());

        energies->push_back(alphas->current());
        energies->push_back(mean_Ek);
        energies->push_back(mean_Ep);
        //position->push_back(alphas->current());
        position->push_back(wavefunction->getParticles(), wavefunction->nparticles(), wavefunction->dimensions());
        //energies->push_back(hamiltonian->localEnergy(*wavefunction));
        //energies->push_back(hamiltonian->potentialEnergy(*wavefunction));
        //energies->push_back(hamiltonian->kineticEnergy(*wavefunction));

        alphas->advance();
    }
    energies->shrink();
    position->shrink();
}

void Simulation::run(){return run(parameters.alphas);}
