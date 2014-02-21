
#ifndef MAPPARSER_HPP
#define MAPPARSER_HPP

#include <string>

#include "MoveMap.hpp"

using namespace std;

class MapParser
{
  private:
    string mapFileName;

  public:
    MapParser();
    MapParser(string);
    bool setMapFile(string);
    void parse(MoveMap*) const;
};

#endif
