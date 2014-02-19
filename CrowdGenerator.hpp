
#ifndef CROWDGENERATOR_HPP
#define CROWDGENERATOR_HPP

#include "Crowd.hpp"
#include "MoveMap.hpp"

class CrowdGenerator
{
  private:
    int getRandomX(MoveMap&) const;
    int getRandomY(MoveMap&) const;

  public:
    CrowdGenerator();

    // Generates vertices and places them randomly in the given crowd. The
    // number of vertices generated is equal to the population specified in the
    // given MoveMap.
    void populate(Crowd&, MoveMap&) const;
};

#endif
