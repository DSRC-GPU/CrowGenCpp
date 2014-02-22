
#include <cstdlib>

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

void MapParser::parse(vector<GroupDescriptor>& vec) const
{
  XMLDocument doc;
  doc.LoadFile("testmap.xml");
  const char* nodename = "groupdescriptor";

  const XMLElement *gdxml = doc.FirstChildElement(nodename);
  while (gdxml)
  {
    GroupDescriptor gd;
    gd.population(atoi(gdxml->FirstChildElement("population")->GetText()));
    parseSources(gd, gdxml);
    parseSinks(gd, gdxml);
    gd.width(atoi(gdxml->FirstChildElement("map")->Attribute("width")));
    gd.height(atoi(gdxml->FirstChildElement("map")->Attribute("height")));
    gd.charmap(gdxml->FirstChildElement("map")->GetText());
    vec.push_back(gd);

    gdxml = gdxml->NextSiblingElement(nodename);
  }
}

void MapParser::parseSources(GroupDescriptor& gd, const XMLElement*& gdxml) const
{
  // TODO Put these strings in a seperate namespace/file.
  const char* sourcestr = "source";
  const char* xborderstr = "xborder";
  const char* yborderstr = "yborder";
  const char* startstr = "start";
  const char* endstr = "end";

  const XMLElement* sourcesxml = gdxml->FirstChildElement(sourcestr);
  while (sourcesxml)
  {
    Box sourceBox;
    const XMLElement* xborder = gdxml->FirstChildElement(xborderstr);
    const XMLElement* yborder = gdxml->FirstChildElement(yborderstr);
    sourceBox.lowerX(atoi(xborder->Attribute(startstr)));
    sourceBox.upperX(atoi(xborder->Attribute(endstr)));
    sourceBox.lowerY(atoi(yborder->Attribute(startstr)));
    sourceBox.upperY(atoi(yborder->Attribute(endstr)));
    gd.sources().push_back(sourceBox);

    sourcesxml = sourcesxml->NextSiblingElement(sourcestr);
  }
}

void MapParser::parseSinks(GroupDescriptor& gd, const XMLElement*& gdxml) const
{
  // TODO Put these strings in a seperate namespace/file.
  const char* sinksstr = "sinks";
  const char* xborderstr = "xborder";
  const char* yborderstr = "yborder";
  const char* startstr = "start";
  const char* endstr = "end";

  const XMLElement* sinksxml = gdxml->FirstChildElement(sinksstr);
  while (sinksxml)
  {
    Box sourceBox;
    const XMLElement* xborder = gdxml->FirstChildElement(xborderstr);
    const XMLElement* yborder = gdxml->FirstChildElement(yborderstr);
    sourceBox.lowerX(atoi(xborder->Attribute(startstr)));
    sourceBox.upperX(atoi(xborder->Attribute(endstr)));
    sourceBox.lowerY(atoi(yborder->Attribute(startstr)));
    sourceBox.upperY(atoi(yborder->Attribute(endstr)));
    gd.sources().push_back(sourceBox);

    sinksxml = sinksxml->NextSiblingElement(sinksstr);
  }
}

