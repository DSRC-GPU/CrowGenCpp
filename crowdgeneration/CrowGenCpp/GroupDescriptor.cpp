
#include "GroupDescriptor.hpp"

GroupDescriptor::GroupDescriptor(int p, int w, int h)
{
  this->mm_population = p;
  this->mm_width = w;
  this->mm_height = h;
}

int GroupDescriptor::getCrowdDirection(int x, int y) const
{
  // TODO Determine the actual direction at this location from the parsed file.
  return 6; // People are moving to the right.
}

int GroupDescriptor::height() const
{
  return this->mm_height;
}

int GroupDescriptor::height(int h)
{
  if (h > 0)
  {
    this->mm_height = h;
  }
  return this->mm_height;
}

int GroupDescriptor::width() const
{
  return this->mm_width;
} 

int GroupDescriptor::width(int w)
{
  if (w > 0)
  {
    this->mm_width = w;
  }
  return this->mm_width;
}

int GroupDescriptor::population() const
{
  return this->mm_population;
}

int GroupDescriptor::population(int p)
{
  if (p >= 0)
  {
    this->mm_population = p;
  }
  return this->mm_population;
}
