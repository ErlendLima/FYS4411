// -*- lsst-c++ -*-
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "cpptoml.h"
#include <string>

enum class Sampling { BruteForce, Importance };
enum class SamplingMode { Serial, Random };
enum class InitialPlacement { Random, Grid, Nonoverlap};

class Parameters
{
public:
    Parameters(){};
    Parameters(std::string path){read(path);};
    virtual ~Parameters(){};

    bool read(std::string path);

    /**
       These are const to prevent accidental modifications
       from the outside, but by using a const_cast trick they are
       modified by Parameters::parse()
     */
    const double stepsize            = 0.5;
    const double beta                = 1.0;
    const double alpha               = 1.0;
    const size_t dimensions          = 1;
    const size_t num_particles       = 10;
    const int sample_rate            = 100;
    const bool use_mpi               = false;
    const Sampling sampling          = Sampling::BruteForce;
    const SamplingMode sampling_mode = SamplingMode::Serial;
    const InitialPlacement placement = InitialPlacement::Random;
    const double distance            = 1.0;
    const double radius              = 1.0;
    const size_t mc_cycles           = 10;
    const bool record_observables    = false;
    const size_t record_period       = 10;

    bool hasRead(){return has_read;};

private:
    void parse(std::shared_ptr<cpptoml::table>);
    template <class T>
    void parse(std::shared_ptr<cpptoml::table> toml,
               const std::string &name, const T &var);
    bool has_read = false;
};

#endif /* PARAMETERS_H */
