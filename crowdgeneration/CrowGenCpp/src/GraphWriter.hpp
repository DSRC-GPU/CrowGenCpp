
#ifndef GRAPHWRITER_HPP
#define GRAPHWRITER_HPP

#include <vector>
#include <string>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "tinyxml2-master/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class GraphWriter
{
  private:
    XMLDocument _doc;

    void writeBasics(int, int) ;
    void writeVertices(vector<Vertex>&) ;
    void writeEdges(vector<Edge>&) ;
    void writeEdgeSpell(pair<unsigned int, unsigned int>& lifetime,
      XMLElement* spellsElement);
    void flush(string);

  public:
    GraphWriter();
    void writeGraph(vector<Vertex>&, vector<Edge>&, string);
};

#endif
