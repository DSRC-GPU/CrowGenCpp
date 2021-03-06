
#ifndef SIMULATIONWRITER_HPP
#define SIMULATIONWRITER_HPP

#include <string>

#include "Crowd.hpp"
#include "Vertex.hpp"
#include "tinyxml2-master/tinyxml2.h"

using namespace tinyxml2;

// The SimulationWriter class writes the simulation as run by the MoveSimulator
// to an XML file, to be reused later by other programs.
class SimulationWriter
{
  private:
    bool _initialized;
    XMLDocument _doc;

    void addVertex(XMLElement*,Vertex&);

  public:
    SimulationWriter();
    void initialize();
    void writeOut(Crowd&);
    void wrapUp(string, unsigned int, unsigned int);
};

#endif
