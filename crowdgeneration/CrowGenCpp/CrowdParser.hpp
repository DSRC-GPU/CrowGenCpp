
#ifndef CROWDPARSER_HPP
#define CROWDPARSER_HPP

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Point.hpp"
#include "tinyxml2-master/tinyxml2.h"

using namespace tinyxml2;
using namespace std;

class CrowdParser
{
  private:
    void updateVertices(vector<vector<Vertex>>&, XMLElement*);

  public:
    CrowdParser();
    void parseFile(string filename, vector<vector<Vertex>>&);
};

#endif
