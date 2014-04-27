
#include "ProximityGraphGenerator.hpp"

#include "CrowdParser.hpp"
#include "GraphWriter.hpp"
#include "MoveVisualizer.hpp"

#include <iostream>
#include <cmath>

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

  // TODO Place this somewhere else
  MoveVisualizer mv;
  // TODO Make output filename a program parameter.
  mv.visualize(*simulationrun, "sim.svg");
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
  // TODO Explain this crazy method. Hint: we devide the map into tiles and
  // check for each vertex, the 9 closest tiles, and make edges with all
  // vertices that are close to us, in these 9 tiles.
  vector<Vertex>& tickvertices = simulationrun->at(ticknum);
  for (size_t i = 0; i < tickvertices.size(); i++)
  {
    Vertex& s = tickvertices.at(i);

    int v_w_square = max(0, (int) ceil(s.location().x() / (float) _detectionRange) - 1);
    int v_h_square = max(0, (int) ceil(s.location().y() / (float) _detectionRange) - 1);

    _squares[v_w_square][v_h_square].push_back(&s);
  }


  for (size_t i = 0; i < _wsquares; i++)
  {
    for (size_t j = 0; j < _hsquares; j++)
    {
      vector<Vertex*> current_square = _squares[i][j];
      for (size_t k = 0; k < current_square.size(); k++)
      {
        Vertex* s = current_square.at(k);
        updateVertex(*s, ticknum);

        for (size_t l = 0; l < 2; l++)
        {
          if (i + l < _wsquares)
          {
            for (size_t m = 0; m < 2; m++)
            {
              if (j + m < _hsquares)
              {
                vector<Vertex*> other_square = _squares[i + l][j + m];
                for (size_t n = 0; n < other_square.size(); n++)
                {
                  Vertex* t = other_square.at(n);
                  if (s->location().closeTo(t->location(), _detectionRange))
                  {
                    // Allow false negatives, based on the _falseNeg value.
                    if (falseNegative()) continue;
                    if (s->id() < t->id())
                      updateEdge(*s, *t, ticknum);
                    else
                      updateEdge(*t, *s, ticknum);
                  }
                  else if (falsePositive())
                  {
                    // Allow false positives, based on the _falsePos value.
                    if (s->id() < t->id())
                      updateEdge(*s, *t, ticknum);
                    else
                      updateEdge(*t, *s, ticknum);
                  }
                }
              } 
            }
          }
        }
      }
    }
  }

  for (size_t i = 0; i < _wsquares; i++)
  {
    for (size_t j = 0; j < _hsquares; j++)
    {
      _squares[i][j].clear();
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
