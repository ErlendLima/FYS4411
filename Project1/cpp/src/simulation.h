#ifndef SIMULATION_H
#define SIMULATION_H

#include "parameters.h"
#include "hamiltonian.h"
#include <memory.h>

class Simulation
{
public:
    //Simulation(){};
    Simulation(const std::string& path);
    virtual ~Simulation(){};

    void setup();
    void thermalize();

    Parameters parameters = Parameters();
    std::unique_ptr<Hamiltonian> hamiltonian;
};

#endif /* SIMULATION_H */
