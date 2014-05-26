
#include "Edge.hpp"

using namespace std;

static long long edgeId;

Edge::Edge(Vertex& nsource, Vertex& ntarget): _source(nsource), _target(ntarget),
    _type("undirected")
{
  _id = to_string(edgeId++);
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
  return !_id.compare(e._id);
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

