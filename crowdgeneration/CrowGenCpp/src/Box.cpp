
#include "Box.hpp"

#include <cstdlib>

using namespace std;

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

bool Box::inBox(Point& p) const
{
  return inBox(p.x(), p.y());
}

void Box::getPoint(Point& p) const
{
  p.x(getRandomX());
  p.y(getRandomY());
}

int Box::getRandomX() const
{ 
  int spawnWidth = this->upperX() - this->lowerX();
  int res = this->upperX();
  if (spawnWidth > 0)
  {
    res = rand() % spawnWidth + this->lowerX();
  }
  return res;
}

int Box::getRandomY() const
{
  int spawnHeight = this->upperY() - this->lowerY();
  int res = this->upperY();
  if (spawnHeight > 0)
  {
    res = rand() % spawnHeight + this->lowerY();
  }
  return res;
}

bool Box::equal(Box other) const
{
  return lx == other.lowerX() && ux == other.upperX()
    && ly == other.lowerY() && uy == other.upperY();
}

