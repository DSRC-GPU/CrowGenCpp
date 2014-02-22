
#include "MapParser.hpp"

#include "tinyxml2-master/tinyxml2.h"

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

void MapParser::parse(vector<GroupDescriptor>& vec) const
{
  XMLDocument doc;
  doc.LoadFile("testmap.xml");
  XMLElement gdxml = doc.FirstChildElement("groupdescriptor");
  // TODO Make loop over siblings.

  GroupDescriptor gd;
  gd.population(gdxml.FirstChildElement("population")->GetText());
//gdxml.FirstChildElement("sources"));  FIXME Initialize sources.
  gd.width(gdxml.FirstChildElement("map")->Attribute("width"));
  gd.height(gdxml.FirstChildElement("map")->Attribute("height"));
  gd.charmap(gdxml.FirstChildElement("map")->GetText());
}
