
#include "MoveMap.hpp"

MoveMap::MoveMap(int p, int w, int h)
{
  this->mm_population = p;
  this->mm_width = w;
  this->mm_height = h;
}

int MoveMap::getCrowdDirection(int x, int y) const
{
  // TODO Determine the actual direction at this location from the parsed file.
  return 6; // People are moving to the right.
}

int MoveMap::height() const
{
  return this->mm_height;
}

int MoveMap::height(int h)
{
  if (h > 0)
  {
    this->mm_height = h;
  }
  return this->mm_height;
}

int MoveMap::width() const
{
  return this->mm_width;
} 

int MoveMap::width(int w)
{
  if (w > 0)
  {
    this->mm_width = w;
  }
  return this->mm_width;
}

int MoveMap::population() const
{
  return this->mm_population;
}

int MoveMap::population(int p)
{
  if (p >= 0)
  {
    this->mm_population = p;
  }
  return this->mm_population;
}
