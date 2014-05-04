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
            if (circlemap.find(vertices.at(j)) == circlemap.end())
            {
                QGraphicsEllipseItem* item = new QGraphicsEllipseItem(0,0,10,10);
                qgs->addItem(item);
                circlemap[vertices.at(j)] = item;
            }
        }
    }
}

void GraphManager::drawTick(unsigned int ticknum)
{
    vector<Vertex> vertices = simulation.at(ticknum);
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex v = vertices.at(i);
        qDebug(to_string(v.location().x()).c_str());
        circlemap.at(v)->setPos(v.location().x(), v.location().y());
    }
}
