
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
    int _lastNodeId;

    GroupDescriptor& getGroupDescriptor(Node& v, vector<GroupDescriptor>&)
     const;
    void updateLocation(Node&, GroupDescriptor&);
    void updateLocation(Node&, vector<GroupDescriptor>&);
    bool inSink(Node&, GroupDescriptor&) const;
    bool respawn(Node&, GroupDescriptor&);

  public:
    MoveSimulator();
    void initialize(Crowd&, vector<GroupDescriptor>&);
    void doTick(Crowd&, vector<GroupDescriptor>&);
    void doTick(Crowd&, vector<GroupDescriptor>&, int n);
    void writeToFile(bool);
};

#endif
