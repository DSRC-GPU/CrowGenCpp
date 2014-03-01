
#ifndef SIMULATIONWRITER_HPP
#define SIMULATIONWRITER_HPP

#include "tinyxml2-master/tinyxml2.h"

#include "Crowd.hpp"

#include "Node.hpp"

using namespace tinyxml2;

// The SimulationWriter class writes the simulation as run by the MoveSimulator
// to an XML file, to be reused later by other programs.
class SimulationWriter
{
  private:
    bool _initialized;
    XMLDocument _doc;

    void addNode(XMLElement*,Node&);

  public:
    SimulationWriter();
    void initialize();
    void writeOut(Crowd&);
    void wrapUp();
};

#endif
