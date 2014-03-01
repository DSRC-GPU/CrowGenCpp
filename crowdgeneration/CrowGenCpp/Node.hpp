
#ifndef NODE_HPP
#define NODE_HPP

#include "Point.hpp"

/*
 * The Node class represents a single node in a crowd.
 */
class Node
{
  private:
    int _id, _gid;
    int xpos, ypos;

  public:
    Node(int);
    int id() const;
    int id(int nid);
    int gid() const;
    int gid(int ngid);
    int x() const;
    int x(int newx);
    int y() const;
    int y(int newy);
    void updateLocation(Point&);
    bool equal(Node other) const;
};

#endif
