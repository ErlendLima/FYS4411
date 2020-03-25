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
   TODO: Given that only one particle has changed, the computation
         can be simplified
         Do not perform correlation if non-interacting
   TODO: Regn ut Metropolis ratio for seg
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
   Compute the probability ratio used in Metropolis sampling
                  ω = |Ψ_new|²/|Ψ_old|²
 */
double Wavefunction::probabilityRatio() const{
    double onebody_new = oneBodyElement(index_moved);
    double onebody_old = oneBodyElementOriginal();

    double correlation_old = 1;
    double correlation_new = 1;
    if (radius > 0){
        for(size_t i = 0; i < num_particles; i++){
            if(i == index_moved) continue;
            correlation_old *= correlation(cache, particles[i]);
            correlation_new *= correlation(index_moved, i);
        }
    }
    LOGD(onebody_new);
    LOGD(onebody_old);
    LOGD(correlation_old);
    LOGD(correlation_new);
    return SQ((onebody_new * correlation_new) / (onebody_old * correlation_old));
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
    if (radius <= 0)
        return 1;
    double distance_squared = distanceSQ(particles[i], particles[j], num_dimensions);
    if (distance_squared <= radius*radius){
        LOG("ZERO");
        return 0;
    } else {
        return 1 - radius/sqrt(distance_squared);
    }
}

/**
   Compute Jasterow(?) function from raw arrays
*/
double Wavefunction::correlation(const double* p1, const double* p2) const {
    if (radius <= 0)
        return 0;
    double distance_squared = distanceSQ(p1, p2, num_dimensions);
    if (distance_squared <= radius*radius){
        LOG("ZERO");
        return 0;
    } else {
        return 1 - radius/sqrt(distance_squared);
    }
}

inline void Wavefunction::changeCandidate(size_t index, double *new_values) {
    index_moved = index;
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

/**
   Compute the one body element of the candidate particle before
   it was used using cached position
 */
double Wavefunction::oneBodyElementOriginal() const{
    double xyz_sum = SQ(cache[0]);
    if (num_dimensions >= 2)
        xyz_sum += SQ(cache[1]);
    if (num_dimensions >= 3)
        xyz_sum += SQ(beta*cache[2]);
    return exp(-alpha*xyz_sum);
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
