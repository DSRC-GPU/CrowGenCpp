#include "graphmanager.h"
#include "../../crowdgeneration/CrowGenCpp/src/CrowdParser.hpp"

GraphManager::GraphManager()
{
}

void GraphManager::init(QGraphicsScene* qgs)
{
    this->qgs = qgs;
}

void GraphManager::addVertices(std::string filename)
{
    CrowdParser cp;
    cp.parseFile(filename, simulation);
    for (size_t i = 0; i < simulation.size(); i++)
    {
        vector<Vertex> vertices = simulation.at(i);
        for (size_t j = 0; j < vertices.size(); j++)
        {
            if (linemap.find(vertices.at(j)) == linemap.end())
            {
                QGraphicsLineItem* item = new QGraphicsLineItem();
                qgs->addItem(item);
                linemap[vertices.at(j)] = item;
            }
        }
    }
}
