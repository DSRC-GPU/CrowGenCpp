
#include "MoveSimulator.hpp"

MoveSimulator::MoveSimulator()
{
  // Nothing to do here.
}

void MoveSimulator::doTick(Crowd& c, MoveMap& mm) const
{
  for (int i = 0; i < c.size(); i++)
  {
    this->updateLocation(c.at(i), mm);
  }
}

void MoveSimulator::doTick(Crowd& c, MoveMap& mm, int n) const
{
  for (int i = 0; i < n; i++)
  {
    this->doTick(c, mm);
  }
}

void MoveSimulator::updateLocation(Vertex& v, MoveMap& mm) const
{
  vector<int>& xs = lookupXDirectionVector(mm.getCrowdDirection(v.x(), v.y()));
  vector<int>& ys = lookupYDirectionVector(mm.getCrowdDirection(v.x(), v.y()));

  // Change the position of the vertex to its current position + a random
  // possible offset selected from the vector of possible offsets (based on its
  // direction).
  v.x(v.x() + xs.at(xs.begin() + (rand() % xs.size())));
  v.y(v.y() + ys.at(ys.begin() + (rand() % ys.size())));
}

vector<int>& lookupXDirectionVector(int direction)
{
  // FIXME Lookup the correct vector.
  return 0;
}

vector<int>& lookupYDirectionVector(int direction)
{
  // FIXME Lookup the correct vector.
  return 0;
}
