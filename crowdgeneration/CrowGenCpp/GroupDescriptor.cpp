
#include "GroupDescriptor.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

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
  int col = calcCharCol(x);
  int row = calcCharRow(y);

  int offset = col + (_cols + 1) * row; 
  char dir = _charmap[offset];
  int res = dir - '0';
  if (res < 0 && res > 9)
    throw out_of_range("Read character was not an ascii-number.");
  return res;
}

int GroupDescriptor::calcCharCol(int x) const
{
  int gridsize = _width / _cols;
  int res = min(_cols - 1, x / gridsize);
  return res;
}

int GroupDescriptor::calcCharRow(int y) const
{
  int gridsize = _height / _rows;
  int res = min(_rows - 1, y / gridsize);
  return res;
}

int GroupDescriptor::gid() const
{
  return _gid;
}

int GroupDescriptor::gid(int ngid)
{
  _gid = ngid;
  return _gid;
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

int GroupDescriptor::rows() const
{
  return _rows;
}

int GroupDescriptor::rows(int nrows)
{
  _rows = nrows;
  return _rows;
}

int GroupDescriptor::cols() const
{
  return _cols;
}

int GroupDescriptor::cols(int ncols)
{
  _cols = ncols;
  return _cols;
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

string GroupDescriptor::charmap() const
{
  return this->_charmap;
}

string GroupDescriptor::charmap(const char* cmap)
{
  string rawmap(cmap);
  rawmap.erase(std::remove(rawmap.begin(), rawmap.end(), ' '), rawmap.end());
  rawmap.erase(std::remove(rawmap.begin(), rawmap.begin() + 1, '\n'),
      rawmap.begin() + 1);
  this->_charmap = rawmap;
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

