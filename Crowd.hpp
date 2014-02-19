
#ifndef CROWD_HPP
#define CROWD_HPP

#include <vector>

#include "Vertex.hpp"

using namespace std;

class Crowd
{
  private:
    vector<Vertex>* vertices;

  public:
    Crowd();
    int size() const;
    vector<Vertex>::iterator begin() const;
    vector<Vertex>::iterator end() const;
    bool add(Vertex);
    bool remove(Vertex);
};

#endif
