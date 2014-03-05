
#include "CrowdParser.hpp"

#include <cstdlib>
#include <iostream>

CrowdParser::CrowdParser(): _uniqueNum(0)
{
}

void CrowdParser::parseFile(string filename, vector<vector<Vertex>>& simulation, bool keepPosition)
{
  XMLDocument doc;
  doc.LoadFile(filename.c_str());

  XMLElement* tick = doc.FirstChildElement("simulation")
    ->FirstChildElement("ticks")->FirstChildElement("tick");

  while (tick)
  {
    updateVertices(simulation, tick, keepPosition);
    tick = tick->NextSiblingElement("tick");
  }
}

void CrowdParser::updateVertices(vector<vector<Vertex>>& simulation,
    XMLElement* tick, bool keepPosition)
{
  int ticknum = atoi(tick->Attribute("num"));
  XMLElement* vertex = tick->FirstChildElement("vertex");
  vector<Vertex> currentVector;

  while (vertex)
  {
    int id = 0;
    if (keepPosition)
      id = rand();
    else
      id = atoi(vertex->Attribute("id"));

    int label = atoi(vertex->Attribute("gid"));
    XMLElement* position = vertex->FirstChildElement("position");
    int x = atoi(position->Attribute("x"));
    int y = atoi(position->Attribute("y"));
    Vertex v(id, label);
    v.start(ticknum);
    v.end(ticknum);
    Point p(x,y);
    v.location(p);
    currentVector.push_back(v);

    vertex = vertex->NextSiblingElement("vertex");
  }

  simulation.push_back(currentVector);
}

