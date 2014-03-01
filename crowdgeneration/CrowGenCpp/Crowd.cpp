
#include <cstdlib>

#include "Crowd.hpp"

using namespace std;

Crowd::Crowd(): _age(0), vertices(new vector<Node>())
{
  // Nothing to do here.
}

int Crowd::size() const
{
  return this->vertices->size();
}

Node& Crowd::at(int i) const
{
  if (i < this->vertices->size())
  {
    return this->vertices->at(i);
  }
  exit(EXIT_FAILURE);
}

vector<Node>& Crowd::getVertices() const
{
  return *this->vertices;
}

bool Crowd::add(Node v)
{
  this->vertices->push_back(v);
  return true;
}

bool Crowd::remove(Node v)
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

int Crowd::age() const
{
  return this->_age;
}

int Crowd::age(int n)
{
  this->_age = n;
  return this->_age;
}
