
#include "Node.hpp"

Node::Node(int vid): xpos(0), ypos(0)
{
  this->_id = vid;
}

int Node::id() const
{
  return _id;
}

int Node::id(int nid)
{
  _id = nid;
  return _id;
}

int Node::gid() const
{
  return _gid;
}

int Node::gid(int ngid)
{
  _gid = ngid;
  return _gid;
}

int Node::x() const
{
  return xpos;
}

int Node::x(int newx)
{
  if (newx >= 0)
    this->xpos = newx;
  return newx;
}

int Node::y() const
{
  return ypos;
}

int Node::y(int newy)
{
  if (newy >= 0)
    this->ypos = newy;
  return newy;
}

void Node::updateLocation(Point& p)
{
  x(p.x());
  y(p.y());
}

bool Node::equal(Node other) const
{
  return this->_id == other._id;
}
