
#include "GraphWriter.hpp"

GraphWriter::GraphWriter()
{
}

void GraphWriter::writeGraph(vector<Vertex>& vertices, vector<Edge>& edges) const
{
  writeBasics();
  writeVertices(vertices);
  writeEdges(edges);
}

void GraphWriter::writeBasics() const
{
  // FIXME Implement.
}

void GraphWriter::writeVertices(vector<Vertex>& vertices) const
{
  // FIXME Implement.
}

void GraphWriter::writeEdges(vector<Edge>& edges) const
{
  // FIXME Implement.
}
