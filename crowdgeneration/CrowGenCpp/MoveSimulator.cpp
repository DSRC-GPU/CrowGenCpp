
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "MoveSimulator.hpp"
#include "directions.hpp"

MoveSimulator::MoveSimulator()
{
  // Nothing to do here.
}

void MoveSimulator::doTick(Crowd& c, vector<GroupDescriptor>& mm) const
{
  for (int i = 0; i < c.size(); i++)
  {
    this->updateLocation(c.at(i), mm);
    // FIXME Check new position, is it a sink?
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
  cout << v.x() << "," << v.y() << endl;

  // FIXME Do we leave the map? -> Bounce!
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

