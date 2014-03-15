
#include <cstdlib>
#include <iostream>

#include "CrowdGenerator.hpp"
#include "Point.hpp"

using namespace std;

CrowdGenerator::CrowdGenerator(): _lastid(0)
{
  // Nothing to do here.
}

void CrowdGenerator::populate(Crowd& c, vector<GroupDescriptor>& gds)
{
  for (unsigned int i = 0; i < gds.size(); i++)
  {
    populate(c, gds.at(i));
  }
}

void CrowdGenerator::populate(Crowd& c, GroupDescriptor& gd)
{
  for (int i = 0; i < gd.population(); i++)
  {
    Vertex v(_lastid++);
    Point p;
    gd.spawn().getPoint(p);
    v.location(p);
    v.label(gd.gid());
    c.add(v);
  }
}

