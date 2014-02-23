
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Point.hpp"

/*
 * The Vertex class represents a single node in a crowd.
 */
class Vertex
{
  private:
    int _id, _gid;
    int xpos, ypos;

  public:
    Vertex(int);
    int id() const;
    int id(int nid);
    int gid() const;
    int gid(int ngid);
    int x() const;
    int x(int newx);
    int y() const;
    int y(int newy);
    void updateLocation(Point&);
    bool equal(Vertex other) const;
};

#endif
