
#include "Crowd.hpp"

Crowd::Crowd(): vertices(new vector<Vertex>())
{
  // Nothing to do here.
}

int Crowd::size() const
{
  return this->vertices->size();
}

vector<Vertex>::iterator Crowd::begin() const
{
  return this->vertices->begin();
}

vector<Vertex>::iterator Crowd::end() const
{
  return this->vertices->end();
}

bool Crowd::add(Vertex v)
{
  this->vertices->push_back(v);
  return true;
}

bool Crowd::remove(Vertex v)
{
  for (size_t i = 0; i < size(); i++)
  {
    if (v.equal(this->vertices->at(i)))
    {
      this->vertices->erase(this->vertices->begin() + i);
      return true;
    }
  }
  return false;
}
