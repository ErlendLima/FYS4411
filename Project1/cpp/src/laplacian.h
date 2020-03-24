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
    // Spacing
    //double h = 1e-14;
    const double h = 1e-4;
};

class LaplacianAnalytical : public Laplacian {
public:
  double operator()( Wavefunction &) override;
};

class LaplacianAnalyticalNonInteracting : public Laplacian {
public:
    double operator()(Wavefunction &) override;
};
#endif /* LAPLACIAN_H */
