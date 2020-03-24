#include "metropolis.h"
#include <random>
#include "macros.h"
#include <iostream>


Metropolis::Metropolis(int _seed, int upper_bound){
    if (_seed == -1){
        std::random_device rd;
        _seed = rd();
    }
    seed = _seed;
    gen = std::mt19937_64(seed);
    normal_distribution = std::normal_distribution<double>(0.0, 1.0);
    uniform_distribution = std::uniform_real_distribution<double>(-1.0, 1.0);
    theta_distribution = std::uniform_real_distribution<double>(0.0, 1.0);
    uniform_int_distribution = std::uniform_int_distribution<int>(0, upper_bound);
}

template <> double Metropolis::draw<double>() {
  return uniform_distribution(gen);
}

template <> int Metropolis::draw<int>() {
  return uniform_int_distribution(gen);
}

bool Metropolis::step(Distribution& dist){
    // Draw the index in 0..length ensured by construction
    int index = draw<int>();
    LOGD(index);
    // Draw the random numbers required by the distribution
    draw(cache, dist.dimensions());

    double P_old = dist.probabilityDensity();
    // Give the distribution the values, assigning it the job
    // of using them correctly and restoring the old values
    // if the change is rejected.
    dist.changeCandidate(index, cache);
    double P_new = dist.probabilityDensity();

    //double w = P_new / P_old;
    double w = dist.probabilityRatio();
    LOGD(P_old);
    LOGD(P_new);
    LOGD(w);

    if (drawTheta() <= w)
      return true;

    dist.restoreCandidate(index);
    return false;
}
