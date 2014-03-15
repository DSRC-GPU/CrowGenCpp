
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Point.hpp"

// This class represents a vertex in a gexf proximity graph.
class Vertex
{
  private:
    int _id, _label;
    int _start, _end;
    int _token;
    Point _p;

  public:
    Vertex();
    Vertex(int);
    Vertex(int, int);

    int id() const;
    int id(int);
    int label() const;
    int label(int);
    int start() const;
    int start(int);
    int end() const;
    int end(int);
    int token() const;
    bool alive() const;
    void alive(bool);
    Point& location();
    void location(Point);
    bool operator==(Vertex&) const;
};

#endif

