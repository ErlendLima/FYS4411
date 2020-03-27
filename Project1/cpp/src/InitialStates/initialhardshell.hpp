#pragma once
#include "initialstate.hpp"
#include "../particles.hpp"

class InitialHardshell : public InitialState
{
public:
    void initiate(); //Initialize Particles object
    void normalSpred(double std);
};
