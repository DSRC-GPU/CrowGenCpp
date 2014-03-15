
#ifndef CROWD_HPP
#define CROWD_HPP

#include <vector>

#include "Vertex.hpp"

using namespace std;

// The Crowd class is a container that holds all Vertex objects (nodes in the
// crowd).
class Crowd
{
  private:
    int _age;
    vector<Vertex>* vertices;

  public:
    Crowd();
    unsigned int size() const;
    Vertex& at(unsigned int) const;
    vector<Vertex>& getVertices() const;
    bool add(Vertex);
    bool remove(Vertex);
    int age() const;
    int age(int);
};

#endif
