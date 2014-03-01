
#ifndef GRAPHWRITER_HPP
#define GRAPHWRITER_HPP

#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "tinyxml2-master/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class GraphWriter
{
  private:
    XMLDocument _doc;

    void writeBasics() ;
    void writeVertices(vector<Vertex>&) ;
    void writeEdges(vector<Edge>&) ;
    void flush();

  public:
    GraphWriter();
    void writeGraph(vector<Vertex>&, vector<Edge>&) ;
};

#endif
