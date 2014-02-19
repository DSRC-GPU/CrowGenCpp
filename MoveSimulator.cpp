
#include "MoveSimulator.hpp"

MoveSimulator::MoveSimulator()
{
  // Nothing to do here.
}

void MoveSimulator::doTick(Crowd& c, MoveMap& mm) const
{
  // TODO Do a move tick!
}

void MoveSimulator::doTick(Crowd& c, MoveMap& mm, int n) const
{
  for (int i = 0; i < n; i++)
  {
    this->doTick(c, mm);
  }
}
