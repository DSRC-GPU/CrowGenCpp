#include "graphmanager.h"
#include "../../crowdgeneration/CrowGenCpp/src/CrowdParser.hpp"
#include "edgeparser.h"

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

void GraphManager::addEdges(string filename)
{
    EdgeParser ep;
    ep.parseFile(filename, edges);
    for (size_t i = 0; i < edges.size(); i++)
    {
        QGraphicsLineItem* item = new QGraphicsLineItem(0,0,0,0);
        qgs->addItem(item);
        linemap[edges.at(i)] = item;
    }
}

void GraphManager::drawTick(unsigned int ticknum)
{
    // TODO Make all vertices invisible.
    vector<Vertex> vertices = simulation.at(ticknum % simulation.size());
    for (size_t i = 0; i < vertices.size(); i++)
    {
        Vertex v = vertices.at(i);
        // TODO Make vertex v visible.
        qDebug(to_string(v.location().x()).c_str());
        circlemap.at(v)->setPos(v.location().x(), v.location().y());
    }
    for (size_t i = 0; i < edges.size(); i++)
    {
        Edge e = edges.at(i);
        if (e.start() <= ticknum && ticknum <= e.end())
        {
            linemap.at(e)->line().setP1(circlemap.at(e.source())->pos());
            linemap.at(e)->line().setP2(circlemap.at(e.target())->pos());
        }
    }
}
