
#ifndef CROWDGENERATOR_HPP
#define CROWDGENERATOR_HPP

#include "Crowd.hpp"
#include "GroupDescriptor.hpp"

class CrowdGenerator
{
  private:
    void populate(Crowd&, GroupDescriptor&) const;

  public:
    CrowdGenerator();

    // Generates vertices and places them randomly in the given crowd. The
    // number of vertices generated is equal to the population specified in the
    // given GroupDescriptor.
    void populate(Crowd&, vector<GroupDescriptor>&) const;
};

#endif
