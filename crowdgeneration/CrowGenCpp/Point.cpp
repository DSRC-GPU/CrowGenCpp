
#include "Point.hpp"

Point::Point(): _x(0), _y(0)
{

}

Point::Point(int x, int y)
{
  _x = x;
  _y = y;
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
