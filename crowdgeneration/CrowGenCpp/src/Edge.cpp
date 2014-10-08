
#include "Edge.hpp"

using namespace std;

Edge::Edge(Vertex& nsource, Vertex& ntarget): _source(nsource), _target(ntarget),
    _type("undirected")
{
  _id = to_string(rand()); // TODO Ensure uniqueness.
}

Edge::Edge(const Edge& e): _source(e._source), _target(e._target)
{
  _id = e._id;
  _type = e._type;
  _lifetimes = vector<pair<unsigned int, unsigned int>>(e._lifetimes);
}

string Edge::id() const
{
  return _id;
}

Vertex Edge::source() const
{
  return Vertex(_source);
}

Vertex Edge::target() const
{
  return Vertex(_target);
}

vector<pair<unsigned int,unsigned int>>& Edge::lifetimes()
{
  return _lifetimes;
}

vector<pair<unsigned int, unsigned int>>::iterator Edge::getLifetimeWithEnd(
    unsigned int end)
{
  for (vector<pair<unsigned int, unsigned int>>::iterator it = _lifetimes
      .begin(); it != _lifetimes.end(); it++)
  {
    if ((*it).second == end)
    {
      return it;
    }
  }
  return _lifetimes.end();
}

string Edge::type() const
{
  return _type;
}

void Edge::type(string ntype)
{
  _type = ntype;
}

bool Edge::operator==(const Edge& e) const
{
  return _source == e._source && _target == e._target;
}

Edge& Edge::operator=(const Edge& e)
{
  _source = e._source;
  _target = e._target;
  _lifetimes = e._lifetimes;
  _id = e._id;
  _type = e._type;
  return *this;
}

