
#include "GroupDescriptor.hpp"

GroupDescriptor::GroupDescriptor(int p, int w, int h)
{
  this->_population = p;
  this->_width = w;
  this->_height = h;
}

GroupDescriptor::GroupDescriptor()
{

}

int GroupDescriptor::getCrowdDirection(int x, int y) const
{
  // TODO Determine the actual direction at this location from the parsed file.
  return 6; // People are moving to the right.
}

int GroupDescriptor::height() const
{
  return this->_height;
}

int GroupDescriptor::height(int h)
{
  if (h > 0)
  {
    this->_height = h;
  }
  return this->_height;
}

int GroupDescriptor::width() const
{
  return this->_width;
} 

int GroupDescriptor::width(int w)
{
  if (w > 0)
  {
    this->_width = w;
  }
  return this->_width;
}

int GroupDescriptor::population() const
{
  return this->_population;
}

int GroupDescriptor::population(int p)
{
  if (p >= 0)
  {
    this->_population = p;
  }
  return this->_population;
}

const char* GroupDescriptor::charmap() const
{
  return this->_charmap;
}

const char* GroupDescriptor::charmap(const char* cmap)
{
  this->_charmap = cmap;
  return this->_charmap;
}

Box& GroupDescriptor::spawn()
{
  return _spawn;
}

void GroupDescriptor::spawn(Box nbox)
{
  _spawn = nbox;
}

vector<Box>& GroupDescriptor::sources()
{
  return _sources;
}

vector<Box>& GroupDescriptor::sinks()
{
  return _sinks;
}

