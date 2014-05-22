
#ifndef MOVESIMULATOR_HPP
#define MOVESUMULATOR_HPP

#include <exception>
#include <vector>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "Vertex.hpp"
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
    vector<Vertex> _oldVertices;
    vector<Vertex> _newVertices;
    bool _writeToFile;
    int _lastVertexId;
    int _maxx, _maxy;

    GroupDescriptor& getGroupDescriptor(Vertex& v, vector<GroupDescriptor>&)
     const;
    void updateLocation(Vertex&, GroupDescriptor&);
    void updateLocation(Vertex&, vector<GroupDescriptor>&);
    float getMovementValue(const vector<float>& dirRange);
    bool inSink(Vertex&, GroupDescriptor&) const;
    bool respawn(Vertex&, GroupDescriptor&);

  public:
    MoveSimulator();
    void initialize(Crowd&, vector<GroupDescriptor>&);
    void doTick(Crowd&, vector<GroupDescriptor>&);
    void doTick(Crowd&, vector<GroupDescriptor>&, int, string);
    void writeToFile(bool);
};

#endif
