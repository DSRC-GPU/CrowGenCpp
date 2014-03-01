
#include "Point.hpp"

#include <cstdlib>

Point::Point(): _x(0), _y(0)
{

}

Point::Point(int nx, int ny)
{
  _x = nx;
  _y = ny;
}

int Point::x() const
{
  return _x;
}

int Point::x(int nx)
{
  _x = nx;
  return _x;
}

int Point::y() const
{
  return _y;
}

int Point::y(int ny)
{
  _y = ny;
  return _y;
}

bool Point::closeTo(Point& other, int max) const
{
  return abs(_x - other.x()) + abs(_y - other.y()) <= max;
}
