
#include "Edge.hpp"

Edge::Edge(Vertex& nsource, Vertex& ntarget): _source(nsource), _target(ntarget),
  _start(0), _end(0), _type("undirected")
{
  _id = to_string(rand()); // TODO Ensure uniqueness.
}

string Edge::id() const
{
  return _id;
}

Vertex& Edge::source()
{
  return _source;
}

Vertex& Edge::target()
{
  return _target;
}

int Edge::start() const
{
  return _start;
}

int Edge::start(int nstart)
{
  _start = nstart;
  return _start;
}

int Edge::end() const
{
  return _end;
}

int Edge::end(int nend)
{
  _end = nend;
  return _end;
}

string Edge::type() const
{
  return _type;
}

void Edge::type(string ntype)
{
  _type = ntype;
}
