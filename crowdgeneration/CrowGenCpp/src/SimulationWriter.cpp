
#include <string>
#include <iostream>

#include "SimulationWriter.hpp"

using namespace std;

SimulationWriter::SimulationWriter()
{
  _initialized = false;
}

void SimulationWriter::initialize()
{
  if (!_initialized)
  {
    XMLElement* rootnode = _doc.NewElement("simulation");
    XMLElement* ticknode = _doc.NewElement("ticks");

    _doc.InsertEndChild(rootnode);
    rootnode->InsertEndChild(ticknode);

    _initialized = true;
  }
}

void SimulationWriter::writeOut(Crowd& c)
{
  XMLElement* ticknode = _doc.FirstChildElement("simulation")
    ->FirstChildElement("ticks")->ToElement();

  XMLElement* currenttick = _doc.NewElement("tick");
  currenttick->SetAttribute("num", c.age());
  ticknode->InsertEndChild(currenttick);

  for (size_t i = 0; i < c.size(); i++)
  {
    addVertex(currenttick, c.at(i));
  }
}

void SimulationWriter::wrapUp(string fout, unsigned int maxx, unsigned int maxy)
{
  _doc.FirstChildElement("simulation")->SetAttribute("width", maxx);
  _doc.FirstChildElement("simulation")->SetAttribute("height",maxy);
  _doc.SaveFile(fout.c_str());
}

void SimulationWriter::addVertex(XMLElement* currenttick, Vertex& v)
{
  XMLElement* vertex = _doc.NewElement("vertex");
  XMLElement* position = _doc.NewElement("position");

  vertex->SetAttribute("id", v.id());
  vertex->SetAttribute("gid", v.label());
  Point& pos = v.location();
  position->SetAttribute("x", pos.x());
  position->SetAttribute("y", pos.y());

  vertex->InsertEndChild(position);
  currenttick->InsertEndChild(vertex);
}
