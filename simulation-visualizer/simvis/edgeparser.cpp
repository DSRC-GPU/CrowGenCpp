#include "edgeparser.h"

EdgeParser::EdgeParser()
{
}

void EdgeParser::parseFile(std::string filename, std::vector<Edge>& vedges)
{
  XMLDocument doc;

  doc.LoadFile(filename.c_str());

  XMLElement* gexf = doc.FirstChildElement("gexf");
  XMLElement* graph = gexf->FirstChildElement("graph");
  XMLElement* edges = graph->FirstChildElement("edges");
  XMLElement* edge = edges->FirstChildElement("edge");

    while (edge)
 {
   updateEdges(vedges, edge);
   edge = edge->NextSiblingElement("edge");
  }
}

void EdgeParser::updateEdges(std::vector<Edge>& edges,
    XMLElement* edge)
{
  int vid, wid, start, end;

  vid = atoi(edge->Attribute("source"));
  wid = atoi(edge->Attribute("target"));

  XMLElement* spell = edge->FirstChildElement("spells")->FirstChildElement("spell");
  Edge e;

  while (spell) {
    start = atoi(spell->Attribute("start"));
    end = atoi(spell->Attribute("end"));

    e.source = vid;
    e.target = wid;
    e.spellStart.push_back(start);
    e.spellStop.push_back(end);
    spell = spell->NextSiblingElement("spell");
  }
  edges.push_back(e);
}

