#include "simulation.h"
#include <stdexcept>


Simulation::Simulation(std::string path){
    parameters = Parameters(path);
}


void Simulation::setup(){
    if (!parameters.hasRead())
        throw std::runtime_error("Read parameters before running");

    
}
