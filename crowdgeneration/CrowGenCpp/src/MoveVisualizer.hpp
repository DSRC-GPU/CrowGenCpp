
#ifndef MOVEVISUALIZER_HPP
#define MOVEVISUALIZER_HPP

#include <string>
#include <vector>
#include <set>
#include <map>

#include "tinyxml2-master/tinyxml2.h"
#include "Vertex.hpp"
#include "Edge.hpp"

using namespace std;
using namespace tinyxml2;

// The move visualizer parses a xml movement file and creates an animated svg
// that visualizes the movement of all nodes.
class MoveVisualizer
{
  private:
    set<int> _addedVertices;
    set<string> _addedEdges;
    map<int, int> _tokenMap;
    XMLDocument _doc;
    XMLElement* findPathVertex(int);
    void writeBasics();
    void updateVertex(Vertex&, int);
    void updateEdge(Edge&, int, int);
    void flush(string);

  public:
    MoveVisualizer();
    void visualize(vector<vector<Vertex>>&, string);
    void visualize(vector<vector<Vertex>>&, vector<Edge>&, string);
};

#endif
