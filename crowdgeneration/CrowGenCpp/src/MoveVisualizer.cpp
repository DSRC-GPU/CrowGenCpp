
#include "MoveVisualizer.hpp"

#include <iostream>

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
void MoveVisualizer::updateVertex(Vertex v, int numTicks)
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

    xml_animateMotion->SetAttribute("dur", (to_string(numTicks / 50) + "s")
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
