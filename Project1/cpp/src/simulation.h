#ifndef SIMULATION_H
#define SIMULATION_H

#include "filemanager.h"
#include "hamiltonian.h"
#include "initializer.h"
#include "metropolis.h"
#include "observable.h"
#include "parameters.h"
#include "alphasearch.h"
#include <memory.h>

class Simulation
{
public:
    //Simulation(){};
    Simulation(const std::string& path);
    virtual ~Simulation();

    void setup();
    Wavefunction* makeWavefunction(double alpha);
    void thermalize();
    void run();
    void run(AlphaSearch*);
    void run(double alpha);
    void resetObservables();

    void memoryUsage() const noexcept;
    void dump() const {filemanager.dumpAll();};
    void setWavefunction(Wavefunction*);

    Parameters parameters = Parameters();
private:
    std::unique_ptr<Hamiltonian> hamiltonian;
    std::shared_ptr<Initializer> initializer;
    Metropolis metropolis;
    FileManager filemanager = FileManager("../data");
    Observable<double>* local_energies;
    Observable<double>* kinetic_energy;
    Observable<double>* potential_energy;
    Observable<double>* energies;
    Observable<double>* position;
    Wavefunction* wavefunction;
    double mean_Ek = 0;
    double mean_Ep = 0;
};

#endif /* SIMULATION_H */
