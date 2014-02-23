
#ifndef SIMULATIONWRITER_HPP
#define SIMULATIONWRITER_HPP

#include "Crowd.hpp"

class SimulationWriter
{
  public:
    SimulationWriter();
    void initialize();
    void writeOut(Crowd&) const;
};

#endif
