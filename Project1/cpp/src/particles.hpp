#pragma once

class Particles
{
public:
    void setSystem(class System* sys);
    System* getSystem(){return m_sys;}

    double& position(int particle, int dim);
    double* position(int particle);
    double* getAdjustPos(); //get position og adjusted particle
    int getMovedParticle(); //return which particle was adjusted last
    void adjustPos(double step, int movedParticle, int dim);
    void proposeAdjustPos(double* step, int movedParticle);
    void commitAdjustPos();

protected:
    double** m_positions;
    double* m_adjustedPos;
    System* m_sys;
    int m_movedParticle;
};
