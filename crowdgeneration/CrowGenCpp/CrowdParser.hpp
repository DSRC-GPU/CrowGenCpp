
#ifndef CROWDPARSER_HPP
#define CROWDPARSER_HPP

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Point.hpp"

using namespace std;

class CrowdParser
{
  public:
    CrowdParser();
    void parseFile(string filename, vector<vector<Vertex>>&) const;
};

#endif
