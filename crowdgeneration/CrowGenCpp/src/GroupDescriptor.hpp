
#ifndef MOVEMAP_HPP
#define MOVEMAP_HPP

#include <vector>
#include <string>

#include "Box.hpp"

using namespace std;

// The GroupDescriptor class describes a group of people that are part of a
// crowd. This class contains information about:
//  1. Where the group of people is initially placed in the crowd (spawn)
//  2. In which direction a member of this group should move, given his current
//      location in the crowd.
//  3. Where vertices from this group can leave the map (sinks).
//  4. Where new vertices from this group will be placed as soon as old vertices
//      hit a sink (sources).
//  5. The number of vertices in the group (population).
class GroupDescriptor
{
  private:
    int _gid;
    int _width, _height;
    int _population;
    int _rows, _cols;
    string _charmap;
    Box _spawn;
    vector<Box> _sources;
    vector<Box> _sinks;

    int calcCharCol(int) const;
    int calcCharRow(int) const;

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
    int rows() const;
    int rows(int);
    int cols() const;
    int cols(int);
    int population() const;
    int population(int p);
    string charmap() const;
    string charmap(const char*);
    Box& spawn();
    void spawn(Box);
    vector<Box>& sources();
    vector<Box>& sinks();
};

#endif
