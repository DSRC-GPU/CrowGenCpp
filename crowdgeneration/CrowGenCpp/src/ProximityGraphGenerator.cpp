
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"
#include "MoveVisualizer.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

ProximityGraphGenerator::ProximityGraphGenerator():  _fieldWidth(0),  _fieldHeight(0),
  _falseNeg(0), _falsePos(0)
{
}

void ProximityGraphGenerator::setDetectionRange(int r)
{
  _detectionRange = r;
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
  cp.parseFile(filename, simulationrun);
  _fieldWidth = cp.width();
  _fieldHeight = cp.height();

  // Calculate the number of squares, and tile the complete area into
  // _detectionRange * _detectionRange tiles. This will help up to determine
  // which vertices are close to each other, later on.
  _wsquares = ceil(_fieldWidth/(float)_detectionRange);
  _hsquares = ceil(_fieldHeight/(float)_detectionRange);
}

// The createGraph analyses the double Vertex vector and translates this into a
// vector of Vertex and Edge objects that will be used to create the gexf
// proximity graph.
void ProximityGraphGenerator::createGraph()
{
  // TODO Implement in a more efficient way, or find another job.
  for (size_t i = 0; i < simulationrun.size(); i++)
  {
    cout << "\rIteration #" << i + 1 << "\t/" << simulationrun.size() << flush;
    graphUpdate(i);
  }
}

// The graphUpdate checks for each Vertex in the graph if it is close to any
// other Vertex. If it finds this is true, it will either update the lifetime of
// the existing edge between the two Vertices, or create a new one if none
// exists.
void ProximityGraphGenerator::graphUpdate(unsigned int ticknum)
{
  vector<Vertex>& tickvertices = simulationrun.at(ticknum);
  sort(tickvertices.begin(), tickvertices.end());

  {
    vector<Vertex> newVertices;
    vector<Edge> newEdges;

    for (size_t i = 0; i < tickvertices.size(); i++)
    {
      Vertex& a = tickvertices.at(i);
      updateVertex(newVertices, a, ticknum);
      for (size_t j = i + 1; j < tickvertices.size(); j++)
      {
        Vertex& b = tickvertices.at(j);
        if (a.location().closeTo(b.location(), _detectionRange)
            && a != b)
        {
          // Allow false negatives, based on the _falseNeg value.
          if (falseNegative()) continue;
          if (a.id() < b.id())
            updateEdge(newEdges, a, b, ticknum);
          else
            updateEdge(newEdges, b, a, ticknum);
        }
        else if (falsePositive())
        {
          // Allow false positives, based on the _falsePos value.
          if (a.id() < b.id())
            updateEdge(newEdges, a, b, ticknum);
          else
            updateEdge(newEdges, b, a, ticknum);
        }
      }
    }

    {
      vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
      edges.insert(newEdges.begin(), newEdges.end());
    }
  }
}

// TODO Be more efficient.
void ProximityGraphGenerator::updateVertex(vector<Vertex>& newVertices, Vertex s,
    int ticknum)
{
  for (size_t i = 0; i < vertices.size(); i++)
  {
    Vertex& v = vertices.at(i);
    if (s == v)
    {
      v.end(ticknum);
      return;
    }
  }
  newVertices.push_back(s);
}

// The updateEdge checks if there is an edge between to two given Vertices that
// ends in the previous tick. If there is, the lifetime of this edge is extended
// to the current tick. If there isn't, a new Edge is created that has a
// lifetime of [ticknum, ticknum].
void ProximityGraphGenerator::updateEdge(vector<Edge>& newEdges, Vertex& s,
    Vertex& t, int ticknum)
{
  Edge search(s, t);
  unordered_set<Edge>::const_iterator edgeIt = edges.find(search);
  if (edgeIt != edges.end())
  {
    Edge e(*edgeIt);
    edges.erase(edgeIt);

    unsigned int end = (unsigned int) ticknum - 1;
    vector<pair<unsigned int, unsigned int>>::iterator it =
     e.getLifetimeWithEnd(end);
    if (it != e.lifetimes().end())
    {
      (*it).second += 1;
    }
    else
    {
      e.lifetimes().push_back(make_pair(ticknum, ticknum));
    }
    edges.insert(e);
  } else {
    Edge ne(s, t);
    ne.lifetimes().push_back(make_pair(ticknum, ticknum));
    newEdges.push_back(ne);
  }
}

void ProximityGraphGenerator::writeGraph(string fout)
{
  GraphWriter gw;
  gw.writeGraph(vertices, edges, fout);
}

void ProximityGraphGenerator::createVisualization()
{
  // TODO Place this somewhere else
  MoveVisualizer mv;
  // TODO Make output filename a program parameter.
  mv.visualize(simulationrun, "sim.svg");
}

bool ProximityGraphGenerator::falseNegative() const
{
  return falseData(_falseNeg);
}

bool ProximityGraphGenerator::falsePositive() const
{
  return falseData(_falsePos);
}

bool ProximityGraphGenerator::falseData(double prob) const
{
  double roll = rand() % 100;
  double bar = prob * 100;
  bool res = roll < bar ? true : false;
  return res;
}
