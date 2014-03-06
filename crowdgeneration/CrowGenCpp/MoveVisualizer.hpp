
#ifndef MOVEVISUALIZER_HPP
#define MOVEVISUALIZER_HPP

#include <string>
#include <vector>
#include <set>

#include "tinyxml2-master/tinyxml2.h"
#include "Vertex.hpp"

using namespace std;
using namespace tinyxml2;

// The move visualizer parses a xml movement file and creates an animated svg
// that visualizes the movement of all nodes.
class MoveVisualizer
{
  private:
    set<int> _addedVertices;
    XMLDocument _doc;
    XMLElement* findPathNode(int);
    void writeBasics();
    void updateVertex(Vertex, int);
    void flush();

  public:
    MoveVisualizer();
    void visualize(vector<vector<Vertex>>&, string);
};

#endif
