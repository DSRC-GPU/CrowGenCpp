
#ifndef CROWDGENERATOR_HPP
#define CROWDGENERATOR_HPP

#include "Crowd.hpp"
#include "GroupDescriptor.hpp"

class CrowdGenerator
{
  private:
    int getRandomX(GroupDescriptor&) const;
    int getRandomY(GroupDescriptor&) const;

  public:
    CrowdGenerator();

    // Generates vertices and places them randomly in the given crowd. The
    // number of vertices generated is equal to the population specified in the
    // given GroupDescriptor.
    // FIXME This method needs to run over multiple descriptors.
    void populate(Crowd&, GroupDescriptor&) const;
};

#endif
