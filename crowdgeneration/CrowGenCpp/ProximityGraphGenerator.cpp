
#include "ProximityGraphGenerator.hpp"

void ProximityGraphGenerator::setFalseNegative(double p)
{
  if (0 <= p && p <= 1)
    _falseNeg = p;
}

void ProximityGraphGenerator::setFalsePositive(double p)
{
  if (0 <= p && p <= 1)
    _falsePos = p;
}

void ProximityGraphGenerator::parseCrowd(string filename)
{
  // FIXME parse input file
}

void ProximityGraphGenerator::createGraph() const
{
  // FIXME Write output graph.
}
