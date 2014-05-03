
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"
#include "MoveVisualizer.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>

// TODO Make a destructor...
ProximityGraphGenerator::ProximityGraphGenerator():  _fieldWidth(0),  _fieldHeight(0),
  _falseNeg(0), _falsePos(0)
{
  simulationrun = new vector<vector<Vertex>>();
  vertices = new vector<Vertex>();
  edges = new vector<Edge>();
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
  cp.parseFile(filename, *simulationrun);
  _fieldWidth = cp.width();
  _fieldHeight = cp.height();

  // Calculate the number of squares, and tile the complete area into
  // _detectionRange * _detectionRange tiles. This will help up to determine
  // which vertices are close to each other, later on.
  _wsquares = ceil(_fieldWidth/(float)_detectionRange);
  _hsquares = ceil(_fieldHeight/(float)_detectionRange);
  _squares = new vector<Vertex*>*[_wsquares];
  for (size_t i = 0; i < _wsquares; i++)
  {
    _squares[i] = new vector<Vertex*>[_hsquares];
  }
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
  vector<Vertex>& tickvertices = simulationrun->at(ticknum);
  sort(tickvertices.begin(), tickvertices.end());

  for (size_t i = 0; i < tickvertices.size(); i++)
  {
    Vertex& a = tickvertices.at(i);
    updateVertex(a, ticknum);
    for (size_t j = i + 1; i < tickvertices.size(); j++)
    {
      Vertex& b = tickvertices.at(j);
      if (a.location().closeTo(b.location(), _detectionRange)
          && a != b)
      {
        // Allow false negatives, based on the _falseNeg value.
        if (falseNegative()) continue;
        if (a.id() < b.id())
          updateEdge(a, b, ticknum);
        else
          updateEdge(b, a, ticknum);
      }
      else if (falsePositive())
      {
        // Allow false positives, based on the _falsePos value.
        if (a.id() < b.id())
          updateEdge(a, b, ticknum);
        else
          updateEdge(b, a, ticknum);
      }
    }
  }
}

// TODO Be more efficient.
void ProximityGraphGenerator::updateVertex(Vertex s, int ticknum)
{
  for (size_t i = 0; i < vertices->size(); i++)
  {
    Vertex& v = vertices->at(i);
    if (s == v)
    {
      v.end(ticknum);
      return;
    }
  }
  vertices->push_back(s);
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

void ProximityGraphGenerator::writeGraph(string fout) const
{
  GraphWriter gw;
  gw.writeGraph(*vertices, *edges, fout);
}

void ProximityGraphGenerator::createVisualization() const
{
  // TODO Place this somewhere else
  MoveVisualizer mv;
  // TODO Make output filename a program parameter.
  mv.visualize(*simulationrun, "sim.svg");
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
