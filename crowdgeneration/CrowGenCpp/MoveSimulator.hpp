
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include <exception>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "SimulationWriter.hpp"

using namespace std;

// The MoveSimulator class simulates the movement of vertices in the crowd. It
// iterates over all vertices in the Crowd and moves them based on their
// corresponding GroupDescriptor.
class MoveSimulator
{
  private:
    CrowdGenerator _cg;
    SimulationWriter _sw;
    bool _writeToFile;

    GroupDescriptor& getGroupDescriptor(Node& v, vector<GroupDescriptor>&)
     const;
    void updateLocation(Node&, GroupDescriptor&) const;
    void updateLocation(Node&, vector<GroupDescriptor>&) const;
    bool inSink(Node&, GroupDescriptor&) const;
    bool respawn(Node&, GroupDescriptor&) const;

  public:
    MoveSimulator();
    void initialize(Crowd&, vector<GroupDescriptor>&) const;
    void doTick(Crowd&, vector<GroupDescriptor>&);
    void doTick(Crowd&, vector<GroupDescriptor>&, int n);
    void writeToFile(bool);
};

#endif
