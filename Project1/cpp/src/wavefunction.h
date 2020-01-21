#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H

#include <memory>

class Wavefunction
{
public:
    Wavefunction();
    virtual ~Wavefunction();

protected:
    double mass = 1.0;
    double omega = 1.0;

    int dimensions;

    std::unique_ptr<double> particles;

};



#endif /* WAVEFUNCTION_H */
