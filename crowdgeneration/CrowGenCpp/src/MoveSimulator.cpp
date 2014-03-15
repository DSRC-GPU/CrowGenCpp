
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "MoveSimulator.hpp"
#include "directions.hpp"

MoveSimulator::MoveSimulator():
  _lastVertexId(0), _maxx(0), _maxy(0)
{
  // Nothing to do here.
}

// Use the CrowdGenerator to fill the Crowd with vertices.
void MoveSimulator::initialize(Crowd& c, vector<GroupDescriptor>& gds)
{
  _cg.populate(c, gds);
  _lastVertexId = c.size();
}

// Simulate a single tick on the given Crowd.
void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm)
{
  for (size_t i = 0; i < c.size(); i++)
  {
    if (c.at(i).alive())
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

// Update the location of a single Vertex in the Crowd.
void MoveSimulator::updateLocation(Vertex& v, vector<GroupDescriptor>&
    descriptors) 
{
  updateLocation(v, getGroupDescriptor(v, descriptors));
}

void MoveSimulator::updateLocation(Vertex& v, GroupDescriptor& mm)
{
  Point& pos = v.location();
  int xindex = mm.getCrowdDirection(pos.x(), pos.y()) - 1;
  int yindex = mm.getCrowdDirection(pos.x(), pos.y()) - 1;

  const vector<int>& xs = directions::DIRS_X[xindex];
  const vector<int>& ys = directions::DIRS_Y[yindex]; 

  // Change the position of the vertex to its current position + a random
  // possible offset selected from the vector of possible offsets (based on its
  // direction).
  Point npos(v.location());
  npos.x(npos.x() + xs.at(rand() % xs.size()));
  npos.y(npos.y() + ys.at(rand() % ys.size()));

  // TODO Keep map boundry records per group, in stead of max.
  if (0 < npos.x() && npos.x() < _maxx && 0 < npos.y() && npos.y() < _maxy)
  {
    v.location(npos);
  }

  if (inSink(v, mm))
    respawn(v, mm);
}

// Get the GroupDescriptor for the given Vertex, based on the Vertex' GroupID.
GroupDescriptor& MoveSimulator::getGroupDescriptor(Vertex& v,
    vector<GroupDescriptor>& descriptors) const
{
  for (unsigned int i = 0; i < descriptors.size(); i++)
  {
    if (descriptors.at(i).gid() == v.label())
    {
      return descriptors.at(i);
    }
  }
  throw invalid_argument("Vertex Group ID not found.");
}

// Returns true iff the given Vertex is located in a sink.
bool MoveSimulator::inSink(Vertex& v, GroupDescriptor& gd) const
{
  for (size_t i = 0; i < gd.sinks().size(); i++)
  {
    Box& sink = gd.sinks().at(i);
    if (sink.inBox(v.location()))
    {
      return true;
    }
  }
  return false;
}

// Moves the given Vertex to a random point within a random spawn point.
// Replaces the Vertex' ID with a new one, to simulate one person leaving the
// area, and a new one entering.
bool MoveSimulator::respawn(Vertex& v, GroupDescriptor& gd)
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
    Vertex nv(v);
    v.alive(false);
    nv.location(p);
    nv.id(_lastVertexId++);

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

