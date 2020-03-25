#include "parameters.h"
#include "macros.h"
#include <iostream>
#include <stdexcept>

template <class T> void copy_override_const(T src, const T &dst) {
  const_cast<T &>(dst) = src;
}

bool Parameters::read(std::string path){
  try {
    std::shared_ptr<cpptoml::table> toml = cpptoml::parse_file(path);
    //LOGD(*toml);
    parse(toml);
    has_read = true;
  } catch (const cpptoml::parse_exception &e) {
    std::cerr << "Failed to parse " << path << ": " << e.what() << std::endl;
    return false;
  }
  return true;
}

void Parameters::parse(std::shared_ptr<cpptoml::table> toml){
    parseQualified<double>(toml, "system.stepsize",          stepsize);
    parseQualified<double>(toml, "external.beta",              beta);
    parseQualified<size_t>(toml, "system.dimensions",        dimensions);
    parseQualified<size_t>(toml, "system.particles",         num_particles);
    //parseQualified<bool>  (toml, "system.pi",               use_mpi);
    parseQualified<double>(toml, "internal.radius",            radius);
    parseQualified<size_t>(toml, "system.mccycles",          mc_cycles);
    parseQualified<double>(toml, "external.omega_ho",          omega_ho);
    parseQualified<double>(toml, "external.omega_z",           omega_z);
    parseQualified<bool>  (toml, "system.laplacian_numerical", laplacian_numerical);
    parseQualified<bool>  (toml, "system.verbose",           verbose);
    parseQualified<bool>  (toml, "record.dorecord", record_observables);
    parseQualified<size_t>(toml, "record.period",   record_period);
    parseQualified<double>(toml, "placement.distance", distance);

    // auto hamil = toml->get_as<std::string>("hamiltonian").value_or("spherical");
    // LOG("Reading hamiltonian: %s", hamil.c_str());
    // if (hamil == "spherical")
    //     copy_override_const(Hamiltonians::Spherical, hamiltonian);
    // else if (hamil == "elliptical")
    //     copy_override_const(Hamiltonians::Elliptical, hamiltonian);
    // else
    //     throw std::invalid_argument("Unsupported value for keyword 'hamiltonian'");

    auto init = toml->get_qualified_as<std::string>("placement.type").value_or("random");
    if (init == "random")
        copy_override_const(InitialPlacement::Random, placement);
    else if (init == "grid")
      copy_override_const(InitialPlacement::Grid, placement);
    else if (init == "nonoverlap")
        copy_override_const(InitialPlacement::Nonoverlap, placement);
    else
        throw std::invalid_argument("Unsupported value for keyword 'placement'");


    // Alpha
    double initial = toml->get_qualified_as<double>("alpha.initial").value_or(1.0);
    std::string method = toml->get_qualified_as<std::string>("alpha.method").value_or("single");
    if (method == "single"){
        alphas = new AlphaSingle(initial);
    } else if (method == "grid") {
        double stop = toml->get_qualified_as<double>("alpha.grid.stop").value_or(1.0);
        double step = toml->get_qualified_as<double>("alpha.grid.step").value_or(0.1);
        alphas = new AlphaGrid(initial, stop, step);
    } else if (method == "gradient descent") {
        
    } else {
        throw std::invalid_argument("Unsupported value for keyword 'method'");
    }
}

template <class T>
void Parameters::parse(std::shared_ptr<cpptoml::table> toml, const std::string& name, const T& var){
    auto val = toml->get_as<T>(name);
    copy_override_const<T>(val.value_or(var), var);
    if (val){
        LOG("Reading %s: %g", name.c_str(), var);
    }
}

template <class T>
void Parameters::parseQualified(std::shared_ptr<cpptoml::table> toml, const std::string& name, const T& var){
    auto val = toml->get_qualified_as<T>(name);
    copy_override_const<T>(val.value_or(var), var);
    if (val){
        LOG("Reading qualified %s: %g", name.c_str(), var);
    }
}
