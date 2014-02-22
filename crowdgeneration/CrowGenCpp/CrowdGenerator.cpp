
#include <cstdlib>

#include "CrowdGenerator.hpp"

using namespace std;

CrowdGenerator::CrowdGenerator()
{
  // Nothing to do here.
}

void CrowdGenerator::populate(Crowd& c, vector<GroupDescriptor>& gds) const
{
  for (int i = 0; i < gds.size(); i++)
  {
    populate(c, gds.at(i));
  }
}

void CrowdGenerator::populate(Crowd& c, GroupDescriptor& gd) const
{
  for (int i = 0; i < gd.population(); i++)
  {
    Vertex v(i);
    v.x(getRandomX(gd));
    v.y(getRandomY(gd));
    c.add(v);
  }
}

int CrowdGenerator::getRandomX(GroupDescriptor& gd) const
{ 
  int i = 0;
  Box& spawnBox = gd.spawn();
  int spawnWidth = spawnBox.upperX() - spawnBox.lowerX();
  int res = spawnBox.upperX();
  if (spawnWidth > 0)
  {
    res = rand() % spawnWidth + spawnBox.lowerX();
  }
  return res;
}

int CrowdGenerator::getRandomY(GroupDescriptor& gd) const
{
  Box& spawnBox = gd.spawn();
  int spawnHeight = spawnBox.upperY() - spawnBox.lowerY();
  int res = spawnBox.upperY();
  if (spawnHeight > 0)
  {
    res = rand() % spawnHeight + spawnBox.lowerY();
  }
  return res;
}
