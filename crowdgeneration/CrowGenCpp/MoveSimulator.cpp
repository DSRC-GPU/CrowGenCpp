
#include <cstdlib>
#include <stdexcept>

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

void MoveSimulator::updateLocation(Vertex& v, GroupDescriptor& mm) const
{
  vector<int>& xs = directions::DIRS_X[mm.getCrowdDirection(v.x(), v.y()) - 1];
  vector<int>& ys = directions::DIRS_X[mm.getCrowdDirection(v.x(), v.y()) - 1]; 

  // Change the position of the vertex to its current position + a random
  // possible offset selected from the vector of possible offsets (based on its
  // direction).
  v.x(v.x() + xs.at(rand() % xs.size()));
  v.y(v.y() + ys.at(rand() % ys.size()));
}

