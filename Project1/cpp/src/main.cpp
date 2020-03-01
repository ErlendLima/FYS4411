#include <iostream>
#include "parameters.h"
#include "wavefunction.h"
#include "simulation.h"

int main(int argc, char const *argv[]){
    std::string filename = "../data/metatest.toml";
    if (argc >= 2)
      filename = std::string(argv[1]);
    auto params = Parameters(filename);
    Simulation* simulation = new Simulation(filename);
    simulation->memoryUsage();
    Wavefunction wavefunction = Wavefunction();
    simulation->setup();
    simulation->run();
    simulation->dump();
    return 0;
}
