
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "MoveSimulator.hpp"
#include "directions.hpp"

MoveSimulator::MoveSimulator():
  _lastNodeId(0), _maxx(0), _maxy(0)
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
  for (size_t i = 0; i < c.size(); i++)
  {
    if (!c.at(i).dead())
      this->updateLocation(c.at(i), mm);
  }

  for (size_t i = 0; i < mm.size(); i++)
  {
    _maxx = max(_maxx, mm.at(i).width());
    _maxy = max(_maxy, mm.at(i).height());
  }

  c.age(c.age() + 1);

  // Remove all vertices that hit a sink.
  for (size_t i = 0; i < _oldVertices.size(); i++)
  {
    c.remove(_oldVertices.at(i));
  }
  _oldVertices.clear();

  // Add new vertices that spawn.
  for (size_t i = 0; i < _newVertices.size(); i++)
  {
    c.add(_newVertices.at(i));
  }
  _newVertices.clear();

  if (_writeToFile)
    _sw.writeOut(c);
}

void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm, int n,
    string fout)
{
  if (_writeToFile)
    _sw.initialize();

  for (int i = 0; i < n; i++)
  {
    this->doTick(c, mm);
  }

  if (_writeToFile)
    _sw.wrapUp(fout, _maxx, _maxy);
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
  int newx = v.x() + xs.at(rand() % xs.size());
  int newy = v.y() + ys.at(rand() % ys.size());

  // TODO Keep map boundry records per group, in stead of max.
  if (0 < newx && newx < _maxx && 0 < newy && newy < _maxy)
  {
    v.x(newx);
    v.y(newy);
  }

  if (inSink(v, mm))
    respawn(v, mm);
}

// Get the GroupDescriptor for the given Node, based on the Node' GroupID.
GroupDescriptor& MoveSimulator::getGroupDescriptor(Node& v,
    vector<GroupDescriptor>& descriptors) const
{
  for (unsigned int i = 0; i < descriptors.size(); i++)
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
    int randomSource = -1;
    while (randomSource < 0)
    {
      randomSource = rand() % sources.size();
      if (sources.at(randomSource).equal(gd.spawn()))
        randomSource = -1;
    }
    Point p;
    sources.at(randomSource).getPoint(p);
    Node nv(v);
    v.dead(true);
    nv.updateLocation(p);
    nv.id(_lastNodeId++);

    // New vertices will be added to a temporary vector, that will be added to
    // the crowd at the end of the current tick iteration.
    _newVertices.push_back(nv);
  }
  return true;
}

void MoveSimulator::writeToFile(bool val)
{
  _writeToFile = val;
}

