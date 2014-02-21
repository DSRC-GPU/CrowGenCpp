
#include <cstdlib>

#include "MoveSimulator.hpp"
#include "directions.hpp"

using namespace std;

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
  vector<int>& xs = directions::DIRS_X[mm.getCrowdDirection(v.x(), v.y()) - 1];
  vector<int>& ys = directions::DIRS_X[mm.getCrowdDirection(v.x(), v.y()) - 1]; 

  // Change the position of the vertex to its current position + a random
  // possible offset selected from the vector of possible offsets (based on its
  // direction).
  v.x(v.x() + xs.at(rand() % xs.size()));
  v.y(v.y() + ys.at(rand() % ys.size()));
}

