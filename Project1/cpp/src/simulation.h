#ifndef SIMULATION_H
#define SIMULATION_H

#include "filemanager.h"
#include "hamiltonian.h"
#include "initializer.h"
#include "metropolis.h"
#include "observable.h"
#include "parameters.h"
#include <memory.h>

class Simulation
{
public:
    //Simulation(){};
    Simulation(const std::string& path);
    virtual ~Simulation();

    void setup();
    Wavefunction makeWavefunction();
    void thermalize();
    void run();

    void memoryUsage() const noexcept;
    void dump() const {filemanager.dumpAll();};

    Parameters parameters = Parameters();
private:
    std::unique_ptr<Hamiltonian> hamiltonian;
    std::shared_ptr<Initializer> initializer;
    Metropolis metropolis;
    FileManager filemanager = FileManager("../data");
    Observable<double>* local_energies;
};

#endif /* SIMULATION_H */
