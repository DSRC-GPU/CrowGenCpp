
#include <cstdlib>

#include "CrowdGenerator.hpp"

using namespace std;

CrowdGenerator::CrowdGenerator()
{
  // Nothing to do here.
}

void CrowdGenerator::populate(Crowd& c, MoveMap& mm) const
{
  // TODO Populate crowd.
  for (int i = 0; i < mm.population(); i++)
  {
    Vertex v(i);
    v.x(getRandomX(mm));
    v.y(getRandomY(mm));
    c.add(v);
  }
}

int CrowdGenerator::getRandomX(MoveMap& mm) const
{
  return rand() % mm.width();
}

int CrowdGenerator::getRandomY(MoveMap& mm) const
{
  return rand() % mm.height();
}
