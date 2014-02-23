
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "MoveSimulator.hpp"
#include "directions.hpp"

MoveSimulator::MoveSimulator()
{
  // Nothing to do here.
}

void MoveSimulator::initialize(Crowd& c, vector<GroupDescriptor>& gds) const
{
  _cg.populate(c, gds);
}

void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm) const
{
  for (int i = 0; i < c.size(); i++)
  {
    this->updateLocation(c.at(i), mm);
  }
}

void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm, int n) const
{
  for (int i = 0; i < n; i++)
  {
    this->doTick(c, mm);
  }
}

void MoveSimulator::updateLocation(Vertex& v, vector<GroupDescriptor>&
    descriptors) const
{
  updateLocation(v, getGroupDescriptor(v, descriptors));
}

void MoveSimulator::updateLocation(Vertex& v, GroupDescriptor& mm) const
{
  cout << "Vertex " << v.id() << " moved from " << v.x() << "," << v.y()
    << " to ";

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

  cout << v.x() << "," << v.y() << endl;
}

GroupDescriptor& MoveSimulator::getGroupDescriptor(Vertex& v,
    vector<GroupDescriptor>& descriptors) const
{
  for (int i = 0; i < descriptors.size(); i++)
  {
    if (descriptors.at(i).gid() == v.gid())
    {
      return descriptors.at(i);
    }
  }
  throw invalid_argument("Vertex Group ID not found.");
}

bool MoveSimulator::inSink(Vertex& v, GroupDescriptor& gd) const
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

bool MoveSimulator::respawn(Vertex& v, GroupDescriptor& gd) const
{
  vector<Box>& sources = gd.sources();
  if (sources.size() > 0)
  {
    int randomSource = rand() % sources.size();
    Point p;
    sources.at(randomSource).getPoint(p);
    v.updateLocation(p);
  }
  return true;
}

