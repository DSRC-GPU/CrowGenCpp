
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "MoveSimulator.hpp"
#include "directions.hpp"

MoveSimulator::MoveSimulator(): _lastNodeId(0)
{
  // Nothing to do here.
}

// Use the CrowdGenerator to fill the Crowd with vertices.
void MoveSimulator::initialize(Crowd& c, vector<GroupDescriptor>& gds)
{
  _cg.populate(c, gds);
  _lastNodeId = c.size();
}

// Simulate a single tick on the given Crowd.
void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm)
{
  for (int i = 0; i < c.size(); i++)
  {
    this->updateLocation(c.at(i), mm);
  }

  c.age(c.age() + 1);

  if (_writeToFile)
    _sw.writeOut(c);
}

void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm, int n)
{
  if (_writeToFile)
    _sw.initialize();

  for (int i = 0; i < n; i++)
  {
    this->doTick(c, mm);
  }

  if (_writeToFile)
    _sw.wrapUp();
}

// Update the location of a single Node in the Crowd.
void MoveSimulator::updateLocation(Node& v, vector<GroupDescriptor>&
    descriptors) 
{
  updateLocation(v, getGroupDescriptor(v, descriptors));
}

void MoveSimulator::updateLocation(Node& v, GroupDescriptor& mm)
{
  int xindex = mm.getCrowdDirection(v.x(), v.y()) - 1;
  int yindex = mm.getCrowdDirection(v.x(), v.y()) - 1;

  const vector<int>& xs = directions::DIRS_X[xindex];
  const vector<int>& ys = directions::DIRS_Y[yindex]; 

  // Change the position of the vertex to its current position + a random
  // possible offset selected from the vector of possible offsets (based on its
  // direction).
  v.x(v.x() + xs.at(rand() % xs.size()));
  v.y(v.y() + ys.at(rand() % ys.size()));

  if (inSink(v, mm))
    respawn(v, mm);
}

// Get the GroupDescriptor for the given Node, based on the Node' GroupID.
GroupDescriptor& MoveSimulator::getGroupDescriptor(Node& v,
    vector<GroupDescriptor>& descriptors) const
{
  for (int i = 0; i < descriptors.size(); i++)
  {
    if (descriptors.at(i).gid() == v.gid())
    {
      return descriptors.at(i);
    }
  }
  throw invalid_argument("Node Group ID not found.");
}

// Returns true iff the given Node is located in a sink.
bool MoveSimulator::inSink(Node& v, GroupDescriptor& gd) const
{
  for (size_t i = 0; i < gd.sinks().size(); i++)
  {
    Box& sink = gd.sinks().at(i);
    if (sink.inBox(v.x(), v.y()))
    {
      return true;
    }
  }
  return false;
}

// Moves the given Node to a random point within a random spawn point.
// Replaces the Node' ID with a new one, to simulate one person leaving the
// area, and a new one entering.
bool MoveSimulator::respawn(Node& v, GroupDescriptor& gd)
{
  vector<Box>& sources = gd.sources();
  if (sources.size() > 0)
  {
    int randomSource = rand() % sources.size();
    Point p;
    sources.at(randomSource).getPoint(p);
    v.id(_lastNodeId++);
    v.updateLocation(p);
  }
  return true;
}

void MoveSimulator::writeToFile(bool val)
{
  _writeToFile = val;
}

