
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include <exception>

#include "Crowd.hpp"
#include "GroupDescriptor.hpp"

using namespace std;

class MoveSimulator
{
  private:
    GroupDescriptor& getGroupDescriptor(Vertex& v, vector<GroupDescriptor>&)
     const;
    void updateLocation(Vertex&, GroupDescriptor&) const;
    void updateLocation(Vertex&, vector<GroupDescriptor>&) const;

  public:
    MoveSimulator();
    // FIXME These methods need to run over vectors of descriptors.
    void doTick(Crowd&, vector<GroupDescriptor>&) const;
    void doTick(Crowd&, vector<GroupDescriptor>&, int n) const;
};

#endif
