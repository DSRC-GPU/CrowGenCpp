
#include "Vertex.hpp"

Vertex::Vertex(int vid): xpos(0), ypos(0)
{
  this->id = vid;
}

int Vertex::x() const
{
  return xpos;
}

int Vertex::x(int newx)
{
  this->xpos = newx;
  return newx;
}

int Vertex::y() const
{
  return ypos;
}

int Vertex::y(int newy)
{
  this->ypos = newy;
  return newy;
}

bool Vertex::equal(Vertex other) const
{
  return this->id == other.id;
}
