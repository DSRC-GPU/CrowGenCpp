
#ifndef SIMULATIONWRITER_HPP
#define SIMULATIONWRITER_HPP

#include "tinyxml2-master/tinyxml2.h"

#include "Crowd.hpp"

#include "Vertex.hpp"

using namespace tinyxml2;

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
    void wrapUp();
};

#endif
