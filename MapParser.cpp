
#include "MapParser.hpp"

MapParser::MapParser(): mapFileName("")
{
  // Nothing to do here.
}

MapParser::MapParser(string filename)
{
  this->mapFileName = filename;
}

bool MapParser::setMapFile(string filename)
{
  // TODO Check if file exists.
  this->mapFileName = filename;
  return true;
}

void MapParser::parse(MoveMap* mm) const
{
  // TODO Parse actual file.
  mm = new MoveMap(10000, 500, 500);
}