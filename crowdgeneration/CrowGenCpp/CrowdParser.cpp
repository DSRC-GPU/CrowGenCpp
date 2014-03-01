
#include "CrowdParser.hpp"

#include <cstdlib>

CrowdParser::CrowdParser()
{
}

void CrowdParser::parseFile(string filename, vector<vector<Vertex>>& simulation)
{
  XMLDocument doc;
  doc.LoadFile(filename.c_str());

  XMLElement* tick = doc.FirstChildElement("simulation")
    ->FirstChildElement("ticks")->FirstChildElement("tick");

  while (tick)
  {
    updateVertices(simulation, tick);
    tick = tick->NextSiblingElement("tick");
  }
}

void CrowdParser::updateVertices(vector<vector<Vertex>>& simulation,
    XMLElement* tick)
{
  int ticknum = atoi(tick->Attribute("ticknum"));
  XMLElement* vertex = tick->FirstChildElement("vertex");
  vector<Vertex> currentVector;
  simulation.push_back(currentVector);

  while (vertex)
  {
    int id = atoi(vertex->Attribute("id"));
    int label = atoi(vertex->Attribute("label"));
    XMLElement* position = vertex->FirstChildElement("position");
    int x = atoi(position->Attribute("x"));
    int y = atoi(position->Attribute("y"));
    Vertex v(id, label);
    v.start(ticknum);
    v.end(ticknum);
    Point p(x,y);
    v.location(p);

    vertex = vertex->NextSiblingElement("vertex");
  }
}

