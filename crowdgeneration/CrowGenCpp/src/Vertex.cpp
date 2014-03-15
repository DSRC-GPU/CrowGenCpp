
#include "Vertex.hpp"

Vertex::Vertex(): _id(0), _label(0), _start(0), _end(0), _alive(true)
{
}

Vertex::Vertex(int nid): _label(0), _start(0), _end(0), _alive(true)
{
  _id = nid;
}

Vertex::Vertex(int nid, int nlabel): _start(0), _end(0), _alive(true)
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

bool Vertex::alive() const
{
  return _alive;
}

void Vertex::alive(bool val)
{
  _alive = val;
}

Point& Vertex::location()
{
  return _p;
}

void Vertex::location(Point p)
{
  _p = p;
}

bool Vertex::operator==(Vertex& v) const
{
  return _id == v.id();
}
