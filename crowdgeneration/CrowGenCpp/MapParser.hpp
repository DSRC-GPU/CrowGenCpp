
#ifndef MAPPARSER_HPP
#define MAPPARSER_HPP

#include <string>
#include <vector>

#include "GroupDescriptor.hpp"

#include "tinyxml2-master/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class MapParser
{
  private:
    string mapFileName;
    void parseSources(GroupDescriptor&, const XMLElement*&) const;
    void parseSinks(GroupDescriptor&, const XMLElement*&) const;

  public:
    MapParser();
    MapParser(string);
    bool setMapFile(string);
    void parse(vector<GroupDescriptor>&) const;
};

#endif
