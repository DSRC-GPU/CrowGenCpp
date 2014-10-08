
#ifndef GRAPHWRITER_HPP
#define GRAPHWRITER_HPP

#include <vector>
#include <unordered_set>
#include <string>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "tinyxml2-master/tinyxml2.h"

using std::vector;
using std::string;
using std::pair;
using std::unordered_set;
using namespace tinyxml2;

class GraphWriter
{
  private:
    XMLDocument _doc;

    void writeBasics(int, int) ;
    void writeVertices(vector<Vertex>&) ;
    void writeEdges(unordered_set<Edge>&) ;
    void writeEdgeSpell(pair<unsigned int, unsigned int>& lifetime,
      XMLElement* spellsElement);
    void flush(string);

  public:
    GraphWriter();
    void writeGraph(vector<Vertex>&, unordered_set<Edge>&, string);
};

#endif
