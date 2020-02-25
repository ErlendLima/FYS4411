#include "wavefunction.h"
#include <math.h>
#include <random>
#include <fstream>
#include <iostream>
#include "macros.h"

static inline double distanceSQ(double* r1, double* r2, size_t dimensions);

Wavefunction::Wavefunction(){
}

Wavefunction::~Wavefunction(){
    if (particles == nullptr)
        return;

    std::cout << "foo" << std::endl;
    for (size_t i = 0; i < num_particles; ++i) {
      delete particles[i];
   }
   std::cout << "foo" << std::endl;
   delete particles;
}

void Wavefunction::initialize(size_t nparticles, size_t ndimensions, Initializer* initializer){
    num_particles = nparticles;
    num_dimensions = ndimensions;

    particles = new double*[num_particles];
    for(size_t i = 0; i < num_particles; i++){
        particles[i] = new double[num_dimensions];
    }
    initializer->place(particles, nparticles, ndimensions);
}

/**
   Compute the square of the wavefunction, |Ψ|² where
        Ψ = Πᵢg(α, β, rᵢ)Πⱼf(a, |rᵢ - rⱼ|)
   where
        g = exp -α(x² + y² + βz²)
        f = (1 - a/|rᵢ - rⱼ|) if |rᵢ - rⱼ| > a
 */
double Wavefunction::probabilityDensity() const{
    double psi = 1.0;
    for (size_t i = 0; i < num_particles; ++i) {
        psi *= oneBodyElement(i);
        for (size_t j = i+1; j < num_particles; ++j) {
            psi *= correlation(i, j);
        }
    }
    return psi*psi;
}


/**
   Compute g = exp(-α(x² + y² + βz²))
   for `index`
 */
double Wavefunction::oneBodyElement(int index) const{
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
double Wavefunction::correlation(int i, int j) const{
    // Compute the squared distance to avoid computing the expensive
    // square root *shudders*
    double distance_squared = distanceSQ(particles[i], particles[j], num_dimensions);
    if (distance_squared <= radius*radius){
        return 0;
    } else {
        return 1 - radius/sqrt(distance_squared);
    }
}

inline void Wavefunction::changeCandidate(int index, double *new_values) {
  for (size_t i = 0; i < num_dimensions; ++i) {
    cache[i] = particles[index][i];
    particles[index][i] += step_length * new_values[i];
  }
}

inline void Wavefunction::restoreCandidate(int index) {
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
