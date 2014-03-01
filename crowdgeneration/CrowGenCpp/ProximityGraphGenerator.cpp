
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"

// TODO Make a destructor...
ProximityGraphGenerator::ProximityGraphGenerator(): _falseNeg(0), _falsePos(0)
{
  simulationrun = new vector<vector<Vertex>>();
  vertices = new vector<Vertex>();
  edges = new vector<Edge>();
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
  cp.parseFile(filename, *simulationrun);
}

// The createGraph analyses the double Vertex vector and translates this into a
// vector of Vertex and Edge objects that will be used to create the gexf
// proximity graph.
void ProximityGraphGenerator::createGraph()
{
  // TODO Implement in a more efficient way, or find another job.
  for (size_t i = 0; i < simulationrun->size(); i++)
  {
    graphUpdate(i);
  }
}

// The graphUpdate checks for each Vertex in the graph if it is close to any
// other Vertex. If it finds this is true, it will either update the lifetime of
// the existing edge between the two Vertices, or create a new one if none
// exists.
void ProximityGraphGenerator::graphUpdate(int ticknum)
{
  for (size_t i = 0; i < vertices->size(); i++)
  {
    // FIXME Update vertex lifetime.
    for (size_t j = 0; j < vertices->size(); j++)
    {
      Vertex& s = vertices->at(i);
      Vertex& t = vertices->at(j);
      // TODO Allow 5 as an external parameter.
      if (t.id() > s.id() && s.location().closeTo(t.location(), 5))
      {
         updateEdge(s, t, ticknum);
      }
    }
  }
}

// The updateEdge checks if there is an edge between to two given Vertices that
// ends in the previous tick. If there is, the lifetime of this edge is extended
// to the current tick. If there isn't, a new Edge is created that has a
// lifetime of [ticknum, ticknum].
void ProximityGraphGenerator::updateEdge(Vertex& s, Vertex& t, int ticknum)
{
  for (size_t i = 0; i < edges->size(); i++)
  {
    Edge& e = edges->at(i);
    if (e.source() == s && e.target() == t)
    {
      if (e.end() == ticknum - 1)
      {
        e.end(ticknum);
        return;
      }
    }
  }
  Edge ne(s, t);
  ne.start(ticknum);
  ne.end(ticknum);
  edges->push_back(ne);
}

void ProximityGraphGenerator::writeGraph() const
{
  GraphWriter gw;
  gw.writeGraph(*vertices, *edges);
}

