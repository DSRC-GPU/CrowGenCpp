
#include "CrowdParser.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

CrowdParser::CrowdParser()
{
}

void CrowdParser::parseFile(string filename, vector<vector<Vertex> >& simulation)
{
  XMLDocument doc;
  doc.LoadFile(filename.c_str());

  XMLElement* sim = doc.FirstChildElement("simulation");
  XMLElement* tick = sim->FirstChildElement("ticks")->FirstChildElement("tick");

  _width = std::stoi(string(sim->Attribute("width")));
  _height = std::stoi(string(sim->Attribute("height")));

  while (tick)
  {
    updateVertices(simulation, tick);
    tick = tick->NextSiblingElement("tick");
  }
}

void CrowdParser::updateVertices(vector<vector<Vertex> >& simulation,
    XMLElement* tick)
{
  int ticknum = atoi(tick->Attribute("num"));
  XMLElement* vertex = tick->FirstChildElement("vertex");
  vector<Vertex> currentVector;

  while (vertex)
  {
    int id = atoi(vertex->Attribute("id"));
    int label = atoi(vertex->Attribute("gid"));
    int ntoken = atoi(vertex->Attribute("token"));
    XMLElement* position = vertex->FirstChildElement("position");
    int x = atoi(position->Attribute("x"));
    int y = atoi(position->Attribute("y"));
    Vertex v(id, label);
    v.start(ticknum);
    v.end(ticknum);
    Point p(x,y);
    v.location(p);
    v.token(ntoken);
    currentVector.push_back(v);

    vertex = vertex->NextSiblingElement("vertex");
  }

  simulation.push_back(currentVector);
}

unsigned int CrowdParser::width() const
{
  return _width;
}

unsigned int CrowdParser::height() const
{
  return _height;
}

