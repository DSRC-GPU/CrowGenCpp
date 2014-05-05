
#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>

#include "Vertex.hpp"

using namespace std;

// This class represents an Edge between to Vertex objects in the gexf proximity
// graph.
class Edge
{
  private:
    Vertex& _source, _target;
    int _start, _end;
    string _id;
    string _type;

  public:
    Edge(Vertex&, Vertex&);

    string id() const;
    Vertex& source();
    Vertex& target();
    int start() const;
    int start(int);
    int end() const;
    int end(int);
    string type() const;
    void type(string);

    bool operator<(const Edge&) const;
    bool operator==(const Edge&) const;
    Edge& operator=(const Edge&);
};

#endif

