
#ifndef BOX_HPP
#define BOX_HPP

#include "Point.hpp"

// The Box class represents a rectangular area.
class Box
{
  private:
    int lx, ux, ly, uy;

  public:
    Box();
    Box(int lowx, int upx, int lowy, int upy);

    int lowerX() const;
    int lowerX(int nx);
    int lowerY() const;
    int lowerY(int ny);

    int upperX() const;
    int upperX(int nx);
    int upperY() const;
    int upperY(int ny);

    // Returns true iff the given coordinates are inside the box boundaries or
    // on the box borders.
    bool inBox(int x, int y) const;
    bool inBox(Point&) const;
    // Calculates a random point within the box boundaries or on the box
    // borders, and writes these coordinates to the given point.
    void getPoint(Point&) const;
    int getRandomX() const;
    int getRandomY() const;
    bool equal(Box) const;
};

#endif
