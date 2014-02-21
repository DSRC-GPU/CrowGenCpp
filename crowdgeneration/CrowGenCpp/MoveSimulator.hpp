
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include "Crowd.hpp"
#include "MoveMap.hpp"

class MoveSimulator
{
  private:
    void updateLocation(Vertex&, MoveMap&) const;

  public:
    MoveSimulator();
    void doTick(Crowd&, MoveMap&) const;
    void doTick(Crowd&, MoveMap&, int n) const;
};

#endif
