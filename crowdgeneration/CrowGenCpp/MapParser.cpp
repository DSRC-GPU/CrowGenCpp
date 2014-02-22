
#include <cstdlib>

#include "MapParser.hpp"

#include "tinyxml2-master/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

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
  const char* nodename = "groupdescriptor";
  const XMLElement *gdxml = doc.FirstChildElement(nodename);

  while (gdxml != NULL)
  {
    GroupDescriptor gd;
    gd.population(atoi(gdxml->FirstChildElement("population")->GetText()));
    //gdxml.FirstChildElement("sources"));  FIXME Initialize sources.
    gd.width(atoi(gdxml->FirstChildElement("map")->Attribute("width")));
    gd.height(atoi(gdxml->FirstChildElement("map")->Attribute("height")));
    gd.charmap(gdxml->FirstChildElement("map")->GetText());
    vec.push_back(gd);
    gdxml = gdxml->NextSiblingElement(nodename);
  }
}
