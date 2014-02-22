
#include "Vertex.hpp"

Vertex::Vertex(int vid): xpos(0), ypos(0)
{
  this->_id = vid;
}

int Vertex::id() const
{
  return _id;
}

int Vertex::id(int nid)
{
  _id = nid;
  return _id;
}

int Vertex::gid() const
{
  return _gid;
}

int Vertex::gid(int ngid)
{
  _gid = ngid;
  return _gid;
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
  return this->_id == other._id;
}
