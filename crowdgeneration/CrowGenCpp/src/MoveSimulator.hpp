
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include <exception>
#include <vector>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "Node.hpp"
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
    vector<Node> _oldVertices;
    vector<Node> _newVertices;
    bool _writeToFile;
    int _lastNodeId;
    int _maxx, _maxy;

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
    void doTick(Crowd&, vector<GroupDescriptor>&, int, string);
    void writeToFile(bool);
};

#endif
