
#ifndef BOX_HPP
#define BOX_HPP

#include "Point.hpp"

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

    bool inBox(int x, int y) const;
    void getPoint(Point&) const;
    int getRandomX() const;
    int getRandomY() const;
};

#endif
