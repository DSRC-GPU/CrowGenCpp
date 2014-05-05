#ifndef EDGEPARSER_H
#define EDGEPARSER_H

#include <string>
#include <vector>
#include "edge.h"
#include "../../crowdgeneration/CrowGenCpp/src/tinyxml2-master/tinyxml2.h"

using namespace tinyxml2;

class EdgeParser
{
public:
    EdgeParser();
    void parseFile(std::string filename, std::vector<Edge>& edges);
    void updateEdges(std::vector<Edge>& edges, XMLElement* edge);
};

#endif // EDGEPARSER_H
