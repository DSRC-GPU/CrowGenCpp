
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "MapParser.hpp"

MapParser::MapParser(): mapFileName("")
{
  // Nothing to do here.
}

MapParser::MapParser(string filename)
{
  this->mapFileName = filename.c_str();
}

bool MapParser::setMapFile(const char* filename)
{
  // TODO Check if file exists.
  this->mapFileName = filename;
  return true;
}

void MapParser::parse(vector<GroupDescriptor>& vec) const
{
  XMLDocument doc;
  int err = doc.LoadFile(mapFileName);
  if (err != XML_NO_ERROR)
  {
    cout << "Error loading file: " << mapFileName << " " << err << endl;
    cout << doc.GetErrorStr1() << endl;
    cout << doc.GetErrorStr2() << endl;
    exit(EXIT_FAILURE);
  }
  const char* nodename = "groupdescriptor";

  const XMLElement *gdxml = doc
    .FirstChildElement("crowd")->FirstChildElement(nodename);

  if (!gdxml)
  {
    cerr << "Error in map file." << endl;
    exit(EXIT_FAILURE);
  }

  while (gdxml)
  {
    GroupDescriptor gd;
    gd.gid(atoi(gdxml->Attribute("gid")));
    gd.population(atoi(gdxml->FirstChildElement("population")->GetText()));
    parseSources(gd, gdxml);
    parseSinks(gd, gdxml);
    gd.width(atoi(gdxml->FirstChildElement("map")->Attribute("width")));
    gd.height(atoi(gdxml->FirstChildElement("map")->Attribute("height")));
    gd.rows(atoi(gdxml->FirstChildElement("map")->Attribute("rows")));
    gd.cols(atoi(gdxml->FirstChildElement("map")->Attribute("cols")));
    gd.charmap(gdxml->FirstChildElement("map")->GetText());
    vec.push_back(gd);

    gdxml = gdxml->NextSiblingElement(nodename);
  }
}

void MapParser::parseSources(GroupDescriptor& gd, const XMLElement*& gdxml) const
{
  // TODO Put these strings in a seperate namespace/file.
  const char* sourcestr = "sources";
  const char* xborderstr = "xborder";
  const char* yborderstr = "yborder";
  const char* startstr = "start";
  const char* endstr = "end";

  const XMLElement* sourcesxml = gdxml->FirstChildElement(sourcestr)
    ->FirstChildElement("source");
  while (sourcesxml)
  {
    Box sourceBox;
    const XMLElement* xborder = sourcesxml->FirstChildElement(xborderstr);
    const XMLElement* yborder = sourcesxml->FirstChildElement(yborderstr);
    sourceBox.lowerX(atoi(xborder->Attribute(startstr)));
    sourceBox.upperX(atoi(xborder->Attribute(endstr)));
    sourceBox.lowerY(atoi(yborder->Attribute(startstr)));
    sourceBox.upperY(atoi(yborder->Attribute(endstr)));
    gd.sources().push_back(sourceBox);

    const char* type = sourcesxml->Attribute("type");
    const char* spawn = "spawn";
    if (type && strcmp(spawn, type) == 0)
    {
      gd.spawn(sourceBox);
    }

    sourcesxml = sourcesxml->NextSiblingElement("source");
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

  const XMLElement* sinksxml = gdxml->FirstChildElement(sinksstr)
    ->FirstChildElement("sink");
  while (sinksxml)
  {
    Box sinkBox;
    const XMLElement* xborder = sinksxml->FirstChildElement(xborderstr);
    const XMLElement* yborder = sinksxml->FirstChildElement(yborderstr);
    sinkBox.lowerX(atoi(xborder->Attribute(startstr)));
    sinkBox.upperX(atoi(xborder->Attribute(endstr)));
    sinkBox.lowerY(atoi(yborder->Attribute(startstr)));
    sinkBox.upperY(atoi(yborder->Attribute(endstr)));
    gd.sinks().push_back(sinkBox);

    sinksxml = sinksxml->NextSiblingElement("sink");
  }
}

