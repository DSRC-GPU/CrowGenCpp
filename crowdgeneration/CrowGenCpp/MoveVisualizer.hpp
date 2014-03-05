
#ifndef MOVEVISUALIZER_HPP
#define MOVEVISUALIZER_HPP

#include <string>
#include <vector>

#include "tinyxml2-master/tinyxml2.h"
#include "Vertex.hpp"

using namespace std;
using namespace tinyxml2;

// The move visualizer parses a xml movement file and creates an animated svg
// that visualizes the movement of all nodes.
class MoveVisualizer
{
  private:
    XMLDocument _doc;
    void writeBasics();
    void updateVertex(Vertex v);
    void flush();

  public:
    MoveVisualizer();
    void visualize(vector<vector<Vertex>>&, string);
};

#endif
