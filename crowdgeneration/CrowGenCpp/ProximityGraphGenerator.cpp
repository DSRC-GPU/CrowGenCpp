
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"

ProximityGraphGenerator::ProximityGraphGenerator(): _falseNeg(0), _falsePos(0)
{
  vertices = new unordered_map<Vertex, Point>();
}

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
  CrowdParser cp;
  cp.parseFile(filename, vertices);
}

void ProximityGraphGenerator::createGraph()
{
  // FIXME Create graph based on parsed data.
}

void ProximityGraphGenerator::writeGraph() const
{
  GraphWriter gw;
  gw.writeFile();
}

