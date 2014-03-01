
#include "Vertex.hpp"

Vertex::Vertex(): _id(0), _label(0), _start(0), _end(0)
{
}

Vertex::Vertex(int nid, int nlabel): _start(0), _end(0)
{
  _id = nid;
  _label = nlabel;
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

int Vertex::label() const
{
  return _label;
}

int Vertex::label(int nlabel)
{
  _label = nlabel;
  return _label;
}

int Vertex::start() const
{
  return _start;
}

int Vertex::start(int nstart)
{
  _start = nstart;
  return _start;
}

int Vertex::end() const
{
  return _end;
}

int Vertex::end(int nend)
{
  _end = nend;
  return _end;
}

Point& Vertex::location()
{
  return _p;
}

void Vertex::location(Point p)
{
  _p = p;
}
