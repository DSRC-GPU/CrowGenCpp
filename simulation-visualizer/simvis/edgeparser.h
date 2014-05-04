#ifndef EDGEPARSER_H
#define EDGEPARSER_H

#include <string>
#include <vector>
#include "../../crowdgeneration/CrowGenCpp/src/Edge.hpp"

class EdgeParser
{
public:
    EdgeParser();
    void parseFile(std::string filename, std::vector<Edge>& edges);
};

#endif // EDGEPARSER_H
