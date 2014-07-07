
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"
#include "MoveVisualizer.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

void ProximityGraphGenerator::PrintGraph(ofstream &file, int ticknum) {
    vector<Vertex>& tickvertices = simulationrun.at(ticknum);
    file << ticknum << "\n";
    file << "NumNodes: " << tickvertices.size() << "\n";

    for (size_t i = 0; i < tickvertices.size(); i++)
    {
        Vertex& a =  tickvertices.at(i);
        Point &loc = a.location();
        file << a.id() << " " << a.label() << " "  << loc.x() << " " << loc.y() << "\n";
    }

    vector<Edge> crr_edges;
    for (size_t i = 0; i < edges.size(); i++)
    {
        Edge& e = edges.at(i);
        unsigned int end = (unsigned int) ticknum;
        vector<pair<unsigned int, unsigned int>>::iterator it = e.getLifetimeWithEnd(end);
        if (it != e.lifetimes().end())
          crr_edges.push_back(e);
    }

    file << "NumEdges: " << crr_edges.size() << "\n";
    for (Edge &e : crr_edges)
      file << e.source().id() << " " << e.target().id() << "\n";
}

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
    ofstream file ;
    file.open("ground_truth.txt");

    // TODO Implement in a more efficient way, or find another job.
    for (size_t i = 0; i < simulationrun.size(); i++)
    {
        graphUpdate(i);
        PrintGraph(file, i);
    }

    file.close();
}

// The graphUpdate checks for each Vertex in the graph if it is close to any
// other Vertex. If it finds this is true, it will either update the lifetime of
// the existing edge between the two Vertices, or create a new one if none
// exists.
void ProximityGraphGenerator::graphUpdate(unsigned int ticknum)
{
  vector<Vertex>& tickvertices = simulationrun.at(ticknum);
  sort(tickvertices.begin(), tickvertices.end());

//#pragma omp parallel
  {
    vector<Vertex> newVertices;
    vector<Edge> newEdges;

//#pragma omp for
    for (size_t i = 0; i < tickvertices.size(); i++)
    {
      Vertex& a = tickvertices.at(i);
      updateVertex(newVertices, a, ticknum);
      for (size_t j = i + 1; j < tickvertices.size(); j++)
      {
        Vertex& b = tickvertices.at(j);
        if (a == b)
          continue;

        if (a.location().closeTo(b.location(), _detectionRange))
        {
          if (a.id() < b.id())
            updateEdge(newEdges, a, b, ticknum);
          else
            updateEdge(newEdges, b, a, ticknum);
        }
      }
    }

//#pragma omp critical
      vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
      edges.insert(edges.end(), newEdges.begin(), newEdges.end());
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
  for (size_t i = 0; i < edges.size(); i++)
  {
    Edge& e = edges.at(i);
    if (e.source() == s && e.target() == t)
    {
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
      return;
    }
  }
  Edge ne(s, t);
  ne.lifetimes().push_back(make_pair(ticknum, ticknum));
  newEdges.push_back(ne);
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
  if (!prob)
    return false;

  double roll = rand() % 100;
  double bar = prob * 100;
  bool res = roll < bar ? true : false;
  return res;
}
