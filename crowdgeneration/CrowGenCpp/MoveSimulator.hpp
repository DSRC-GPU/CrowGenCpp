
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include "Crowd.hpp"
#include "GroupDescriptor.hpp"

class MoveSimulator
{
  private:
    void updateLocation(Vertex&, GroupDescriptor&) const;

  public:
    MoveSimulator();
    // FIXME These methods need to run over vectors of descriptors.
    void doTick(Crowd&, GroupDescriptor&) const;
    void doTick(Crowd&, GroupDescriptor&, int n) const;
};

#endif
