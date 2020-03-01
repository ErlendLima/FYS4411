#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include "wavefunction.h"

class Laplacian
{
public:
    Laplacian(){};
    virtual ~Laplacian(){};
    virtual double operator()(Wavefunction&) = 0;
};

class LaplacianNumerical : public Laplacian
{
public:
    double operator()(Wavefunction&) override;
protected:
    // Step size
     double h = 1e-17;
};

class LaplacianAnalytical : public Laplacian {
public:
  double operator()( Wavefunction &) override;
};
#endif /* LAPLACIAN_H */
