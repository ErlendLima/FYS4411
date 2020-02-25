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
    LOGD(*toml);
    parse(toml);
    has_read = true;
  } catch (const cpptoml::parse_exception &e) {
    std::cerr << "Failed to parse " << path << ": " << e.what() << std::endl;
    return false;
  }
  return true;
}

void Parameters::parse(std::shared_ptr<cpptoml::table> toml){
    parse<double>(toml, "stepsize", stepsize);
    parse<double>(toml, "beta", beta);
    parse<double>(toml, "alpha", alpha);
    parse<int>(toml, "dimensions", dimensions);
    parse<int>(toml, "particles", num_particles);
    parse<bool>(toml, "mpi", use_mpi);
    parse<double>(toml, "distance", distance);
    parse<double>(toml, "radius", radius);

    auto hamil = toml->get_as<std::string>("hamiltonian").value_or("spherical");
    LOG("Reading hamiltonian: %s", hamil.c_str());
    if (hamil == "spherical")
        copy_override_const(Hamiltonians::Spherical, hamiltonian);
    else if (hamil == "elliptical")
        copy_override_const(Hamiltonians::Elliptical, hamiltonian);
    else
        throw std::invalid_argument("Unsupported value for keyword 'hamiltonian'");

    auto init = toml->get_as<std::string>("placement").value_or("random");
    if (init == "random")
        copy_override_const(InitialPlacement::Random, placement);
    else if (init == "grid")
      copy_override_const(InitialPlacement::Grid, placement);
    else if (init == "nonoverlap")
        copy_override_const(InitialPlacement::Nonoverlap, placement);
    else
        throw std::invalid_argument("Unsupported value for keyword 'placement'");
}

template <class T>
void Parameters::parse(std::shared_ptr<cpptoml::table> toml, const std::string& name, const T& var){
    auto val = toml->get_as<T>(name);
    copy_override_const<T>(val.value_or(var), var);
    if (val){
        LOG("Reading %s: %g", name.c_str(), var);
    }
}
