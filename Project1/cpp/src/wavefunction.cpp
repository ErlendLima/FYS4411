#include "wavefunction.h"
#include <math.h>
#include <random>
#include <fstream>
#include <iostream>
#include "macros.h"

static inline double distanceSQ(double* r1, double* r2, size_t dimensions);

Wavefunction::Wavefunction(){
}
Wavefunction::Wavefunction(double _alpha, double _beta, double _radius,
                           double _stepsize):
    alpha(_alpha), beta(_beta), radius(_radius), step_length(_stepsize){}

Wavefunction::~Wavefunction(){
    if (particles == nullptr)
        return;

    for (size_t i = 0; i < num_particles; ++i) {
      delete particles[i];
   }
   delete particles;
}

void Wavefunction::initialize(size_t nparticles, size_t ndimensions, std::shared_ptr<Initializer> initializer){
    num_particles = nparticles;
    num_dimensions = ndimensions;

    particles = new double*[num_particles];
    for(size_t i = 0; i < num_particles; i++){
        particles[i] = new double[num_dimensions];
    }
    initializer->place(particles, nparticles, ndimensions);
}

/**
   Evaluate the wavefunction
        Ψ = Πᵢg(α, β, rᵢ)Πⱼf(a, |rᵢ - rⱼ|)
   where
        g = exp -α(x² + y² + βz²)
        f = (1 - a/|rᵢ - rⱼ|) if |rᵢ - rⱼ| > a
 */
double Wavefunction::evaluate() const{
  double psi = 1.0;
  for (size_t i = 0; i < num_particles; ++i) {
    psi *= oneBodyElement(i);
    for (size_t j = i + 1; j < num_particles; ++j) {
      psi *= correlation(i, j);
    }
  }
  return psi;
}

/**
   Compute the square of the wavefunction |Ψ|²
        Ψ = Πᵢg(α, β, rᵢ)Πⱼf(a, |rᵢ - rⱼ|)
 */
double Wavefunction::probabilityDensity() const{
    double psi = evaluate();
    return psi*psi;
}

/**
   Compute g = exp(-α(x² + y² + βz²))
   for `index`
 */
double Wavefunction::oneBodyElement(size_t index) const{
    double xyz_sum = SQ(particles[index][0]);
    if (num_dimensions >= 2)
        xyz_sum += SQ(particles[index][1]);
    if (num_dimensions >= 3)
        xyz_sum += SQ(beta*particles[index][2]);
    return exp(-alpha*xyz_sum);
}

/**
   Compute the Jasterow function
 */
double Wavefunction::correlation(size_t i, size_t j) const{
    // Compute the squared distance to avoid computing the expensive
    // square root *shudders*
    // TODO Is it possible to skip many calls by using the known configuration?
    double distance_squared = distanceSQ(particles[i], particles[j], num_dimensions);
    if (distance_squared <= radius*radius){
        LOG("ZERO");
        return 0;
    } else {
        return 1 - radius/sqrt(distance_squared);
    }
}

inline void Wavefunction::changeCandidate(size_t index, double *new_values) {
  for (size_t i = 0; i < num_dimensions; ++i) {
    cache[i] = particles[index][i];
    particles[index][i] += step_length * new_values[i];
  }
}

inline void Wavefunction::restoreCandidate(size_t index) {
  for (size_t i = 0; i < num_dimensions; ++i) {
    particles[index][i] = cache[i];
  }
}

static inline double distanceSQ(double* r1, double* r2, size_t dimensions){
    double distanceSQ = 0.0;
    for (size_t i = 0; i < dimensions; ++i)
        distanceSQ += SQ(r1[i] - r2[i]);
    return distanceSQ;
}

void Wavefunction::dump(const std::string& path){
    // Dump xyz

    std::ofstream file;
    file.open(path);
    file << num_particles << "\n\n";
    for (size_t i = 0; i < num_particles; ++i) {
        file << "C";
        file << "\t" << particles[i][0];
        if (num_dimensions >= 2){
            file << "\t" << particles[i][1];
        } else {
            file << "\t" << "0.0";
        }

        if (num_dimensions >= 3) {
          file << "\t" << particles[i][2];
        } else {
          file << "\t"
               << "0.0";
        }
        file << "\n";
    }
    file.close();
}
