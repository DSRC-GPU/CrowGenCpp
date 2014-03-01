
#ifndef CROWDPARSER_HPP
#define CROWDPARSER_HPP

#include <string>
#include <unordered_map>

#include "Vertex.hpp"
#include "Point.hpp"

using namespace std;

class CrowdParser
{
  public:
    CrowdParser();
    void parseFile(string filename, unordered_map<Vertex, Point>&) const;
};

#endif
