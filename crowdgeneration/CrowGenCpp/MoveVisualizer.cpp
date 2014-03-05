
#include "MoveVisualizer.hpp"

MoveVisualizer::MoveVisualizer()
{
}

void MoveVisualizer::visualize(vector<vector<Vertex>>& simulation, string fout)
{
  // FIXME Implement.
  writeBasics();
  for (size_t i = 0; i < simulation.size(); i++)
  {
    vector<Vertex> vertices = simulation.at(i);
    for (size_t j = 0; j < vertices.size(); j++)
    {
      updateVertex(vertices.at(j));
    }
  }
  flush();
}

void MoveVisualizer::writeBasics()
{
  // FIXME Implement.
}

void MoveVisualizer::updateVertex(Vertex v)
{
  // FIXME Implement.
}

void MoveVisualizer::flush()
{
  // FIXME Implement.
}
