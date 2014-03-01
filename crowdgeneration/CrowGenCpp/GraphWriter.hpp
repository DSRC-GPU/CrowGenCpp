
#ifndef GRAPHWRITER_HPP
#define GRAPHWRITER_HPP

#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"

using namespace std;

class GraphWriter
{
  private:
    void writeBasics() const;
    void writeVertices(vector<Vertex>&) const;
    void writeEdges(vector<Edge>&) const;

  public:
    GraphWriter();
    void writeGraph(vector<Vertex>&, vector<Edge>&) const;
};

#endif
