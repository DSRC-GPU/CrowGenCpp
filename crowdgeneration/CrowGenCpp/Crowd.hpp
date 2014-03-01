
#ifndef CROWD_HPP
#define CROWD_HPP

#include <vector>

#include "Node.hpp"

using namespace std;

// The Crowd class is a container that holds all Node objects (nodes in the
// crowd).
class Crowd
{
  private:
    int _age;
    vector<Node>* vertices;

  public:
    Crowd();
    int size() const;
    Node& at(int) const;
    vector<Node>& getVertices() const;
    bool add(Node);
    bool remove(Node);
    int age() const;
    int age(int);
};

#endif
