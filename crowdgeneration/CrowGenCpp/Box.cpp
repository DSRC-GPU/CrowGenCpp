
#include "Box.hpp"

Box::Box() : lx(0), ux(0), ly(0), uy(0)
{
  // Nothing to do here.
}

Box::Box(int lowx, int upx, int lowy, int upy)
{
  lx = lowx;
  ux = upx;
  ly = lowy;
  uy = upy;
}

int Box::lowerX() const
{
  return lx;
}

int Box::lowerX(int nx)
{
  lx = nx;
  return lx;
}

int Box::upperX() const
{
  return ux;
}

int Box::upperX(int nx)
{
  ux = nx;
  return ux;
}

int Box::lowerY() const
{
  return ly;
}

int Box::lowerY(int ny)
{
  ly = ny;
  return ly;
}

int Box::upperY() const
{
  return uy;
}

int Box::upperY(int ny)
{
  uy = ny;
  return uy;
}

bool Box::inBox(int x, int y) const
{
  return lx <= x && x <= ux && ly <= y && y <= uy;
}
