
#ifndef MOVEMAP_HPP
#define MOVEMAP_HPP

#include <vector>

#include "Box.hpp"

using namespace std;

class GroupDescriptor
{
  private:
    int _gid;
    int _width, _height;
    int _population;
    const char *_charmap;
    Box _spawn;
    vector<Box> _sources;
    vector<Box> _sinks;

  public:
    GroupDescriptor(int p, int w, int h);
    GroupDescriptor();

    // Gives the direction (1-9 a la numpad) the crowd is moving in, at the
    // given coordinates on the map.
    int getCrowdDirection(int x, int y) const;

    int gid() const;
    int gid(int);
    int height() const;
    int height(int h);
    int width() const;
    int width(int w);
    int population() const;
    int population(int p);
    const char* charmap() const;
    const char* charmap(const char*);
    Box& spawn();
    void spawn(Box);
    vector<Box>& sources();
    vector<Box>& sinks();
};

#endif
