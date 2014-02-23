
#ifndef MAPPARSER_HPP
#define MAPPARSER_HPP

#include <string>
#include <vector>

#include "GroupDescriptor.hpp"

#include "tinyxml2-master/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

// The MapParser class reads a XML map file and parses its contends to a number
// of GroupDescriptors.
class MapParser
{
  private:
    const char* mapFileName;
    void parseSources(GroupDescriptor&, const XMLElement*&) const;
    void parseSinks(GroupDescriptor&, const XMLElement*&) const;

  public:
    MapParser();
    MapParser(const char*);
    bool setMapFile(const char*);
    void parse(vector<GroupDescriptor>&) const;
};

#endif
