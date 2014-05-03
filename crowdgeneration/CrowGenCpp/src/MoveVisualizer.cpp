
#include "MoveVisualizer.hpp"

#include <iostream>

#define TIME_SLICE 50

MoveVisualizer::MoveVisualizer()
{
}

void MoveVisualizer::visualize(vector<vector<Vertex>>& simulation, string fout)
{
  writeBasics();

  for (size_t i = 0; i < simulation.size(); i++)
  {
    vector<Vertex> vertices = simulation.at(i);
    for (size_t j = 0; j < vertices.size(); j++)
    {
      updateVertex(vertices.at(j), simulation.size());
    }
  }

  flush(fout);
}

void MoveVisualizer::visualize(vector<vector<Vertex>>& simulation,
    vector<Edge>& edges, string fout)
{
  writeBasics();

  for (size_t i = 0; i < simulation.size(); i++)
  {
    vector<Vertex> vertices = simulation.at(i);
    for (size_t j = 0; j < vertices.size(); j++)
    {
      updateVertex(vertices.at(j), simulation.size());
    }

    for (size_t k = 0; k < edges.size(); k++)
    {
      updateEdge(edges.at(k), simulation.size(), i);
    }
  }

  flush(fout);
}

void MoveVisualizer::writeBasics()
{
  XMLElement* xml_svg = _doc.NewElement("svg");
  XMLElement* xml_defs = _doc.NewElement("defs");

  xml_svg->SetAttribute("xmlns","http://www.w3.org/2000/svg");
  xml_svg->SetAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");

  _doc.InsertEndChild(xml_svg);
  xml_svg->InsertEndChild(xml_defs);
}

// TODO clean this up.
void MoveVisualizer::updateVertex(Vertex& v, int numTicks)
{
  if (_addedVertices.find(v.token()) == _addedVertices.end())
  {
    // Vertex not yet added.
    XMLElement* xml_svg = _doc.FirstChildElement("svg");
    XMLElement* xml_defs = xml_svg->FirstChildElement("defs");

    XMLElement* xml_node = _doc.NewElement("circle");
    XMLElement* xml_animateMotion = _doc.NewElement("animateMotion");
    XMLElement* xml_mpath = _doc.NewElement("mpath");

    XMLElement* xml_path = _doc.NewElement("path");

    xml_svg->InsertEndChild(xml_node);
    xml_node->InsertEndChild(xml_animateMotion);
    xml_animateMotion->InsertEndChild(xml_mpath);

    xml_defs->InsertEndChild(xml_path);

    xml_node->SetAttribute("id", v.token());
    xml_node->SetAttribute("r", "5");
    xml_node->SetAttribute("cx", "15");
    xml_node->SetAttribute("cy", "15");
    xml_node->SetAttribute("fill", "slategrey");

    xml_animateMotion->SetAttribute("dur", (to_string(numTicks / TIME_SLICE) + "s")
        .c_str());
    xml_animateMotion->SetAttribute("repeatCount", "indefinite");

    xml_mpath->SetAttribute("xlink:href", ("#p" + to_string(v.token())).c_str());

    xml_path->SetAttribute("id", ("p" + to_string(v.token())).c_str());
    xml_path->SetAttribute("d", "M");

    // TODO Remove id's from set when vertex is removed (hit a sink).
    _addedVertices.insert(v.token());
    _tokenMap[v.token()] = v.id();
  }

  XMLElement* xml_path = findPathVertex(v.token());
  // Checking if this vertex has respawned.
  switch (_tokenMap[v.token()] == v.id())
  {
    case false:
      // Yes, we should jump it to a spawn.
      _tokenMap[v.token()] = v.id();
      xml_path->SetAttribute("d", (string(xml_path->Attribute("d"))
            + " M").c_str());
    case true:
      // We continue our path.
      xml_path->SetAttribute("d", (string(xml_path->Attribute("d"))
            + " " + to_string(v.location().x()) + " "
            + to_string(v.location().y())).c_str());
  }
}

void MoveVisualizer::updateEdge(Edge& e, int numTicks, int curTick)
{
  if (_addedEdges.find(e.id()) == _addedEdges.end())
  {
    // Edge not added yet.
    XMLElement* xml_svg = _doc.FirstChildElement("svg");

    XMLElement* xml_line = _doc.NewElement("line");
    XMLElement* animateX1 = _doc.NewElement("animateTransform");
    XMLElement* animateX2 = _doc.NewElement("animateTransform");
    XMLElement* animateY1 = _doc.NewElement("animateTransform");
    XMLElement* animateY2 = _doc.NewElement("animateTransform");

    xml_line->SetAttribute("id", e.id().c_str());
    xml_line->SetAttribute("x1", "-1");
    xml_line->SetAttribute("x2", "-1");
    xml_line->SetAttribute("y1", "-1");
    xml_line->SetAttribute("y2", "-1");
    xml_line->SetAttribute("style", "stroke:rgb(0,0,0);stroke-width:1;");
    animateX1->SetAttribute("attributeName", "x1");
    animateX2->SetAttribute("attributeName", "x2");
    animateY1->SetAttribute("attributeName", "y1");
    animateY2->SetAttribute("attributeName", "y2");
    animateX1->SetAttribute("type", "translate");
    animateX2->SetAttribute("type", "translate");
    animateY1->SetAttribute("type", "translate");
    animateY2->SetAttribute("type", "translate");
    animateX1->SetAttribute("repeatCount", "indefinite");
    animateX2->SetAttribute("repeatCount", "indefinite");
    animateY1->SetAttribute("repeatCount", "indefinite");
    animateY2->SetAttribute("repeatCount", "indefinite");
    animateX1->SetAttribute("dur", (to_string(numTicks / TIME_SLICE) + "s").c_str());
    animateX2->SetAttribute("dur", (to_string(numTicks / TIME_SLICE) + "s").c_str());
    animateY1->SetAttribute("dur", (to_string(numTicks / TIME_SLICE) + "s").c_str());
    animateY2->SetAttribute("dur", (to_string(numTicks / TIME_SLICE) + "s").c_str());
    animateX1->SetAttribute("values", "");
    animateX2->SetAttribute("values", "");
    animateY1->SetAttribute("values", "");
    animateY2->SetAttribute("values", "");

    xml_svg->InsertEndChild(xml_line);
    xml_line->InsertEndChild(animateX1);
    xml_line->InsertEndChild(animateX2);
    xml_line->InsertEndChild(animateY1);
    xml_line->InsertEndChild(animateY2);

    _addedEdges.insert(e.id());
  }
  else
  {
    XMLElement* edgeLine = findEdgeLine(e.id());
    XMLElement* lineX1 = edgeLine->FirstChildElement("animateTransform");
    XMLElement* lineX2 = lineX1->NextSiblingElement("animateTransform");
    XMLElement* lineY1 = lineX2->NextSiblingElement("animateTransform");
    XMLElement* lineY2 = lineY1->NextSiblingElement("animateTransform");

    lineX1->SetAttribute("values", (string(lineX1->Attribute("values"))
        + ";").c_str());
    lineX2->SetAttribute("values", (string(lineX2->Attribute("values"))
        + ";").c_str());
    lineY1->SetAttribute("values", (string(lineY1->Attribute("values"))
        + ";").c_str());
    lineY2->SetAttribute("values", (string(lineY2->Attribute("values"))
        + ";").c_str());
  }

  XMLElement* edgeLine = findEdgeLine(e.id());
  XMLElement* lineX1 = edgeLine->FirstChildElement("animateTransform");
  XMLElement* lineX2 = lineX1->NextSiblingElement("animateTransform");
  XMLElement* lineY1 = lineX2->NextSiblingElement("animateTransform");
  XMLElement* lineY2 = lineY1->NextSiblingElement("animateTransform");

  lineX1->SetAttribute("values", (string(lineX1->Attribute("values"))
      + to_string(e.source().location().x())).c_str());
  lineX2->SetAttribute("values", (string(lineX2->Attribute("values"))
      + to_string(e.target().location().x())).c_str());
  lineY1->SetAttribute("values", (string(lineY1->Attribute("values"))
      + to_string(e.source().location().y())).c_str());
  lineY2->SetAttribute("values", (string(lineY2->Attribute("values"))
      + to_string(e.target().location().y())).c_str());
}

XMLElement* MoveVisualizer::findEdgeLine(string lineid)
{
  XMLElement* svg = _doc.FirstChildElement("svg");
  XMLElement* line = svg->FirstChildElement("line");

  while(line)
  {
    if (line->Attribute("id", lineid.c_str()))
      return line;
    line = line->NextSiblingElement("line");
  }
  return NULL;
}

Point MoveVisualizer::findLinePoint(string lineid, int side)
{
  if (side == 0)
  {
    return _lineStartPoints[lineid];
  }
  else if (side == 1)
  {
    return _lineEndPoints[lineid];
  }
  Point p;
  return p;
}

XMLElement* MoveVisualizer::findPathVertex(int vtoken)
{
  XMLElement* xml_defs = _doc.FirstChildElement("svg")
   ->FirstChildElement("defs");

  XMLElement* xml_path = xml_defs->FirstChildElement("path");

  while (xml_path)
  {
    if (xml_path->Attribute("id", ("p" + to_string(vtoken)).c_str()))
      return xml_path;

    xml_path = xml_path->NextSiblingElement("path");
  }
  return NULL;
}

void MoveVisualizer::flush(string fout)
{
  _doc.SaveFile(fout.c_str());
}
