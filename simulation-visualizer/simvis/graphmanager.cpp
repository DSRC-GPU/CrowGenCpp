#include "graphmanager.h"
#include "../../crowdgeneration/CrowGenCpp/src/CrowdParser.hpp"
#include "edgeparser.h"

#define CIRCLE_DIM 4

GraphManager::GraphManager()
{
}

void GraphManager::init(QGraphicsScene* qgs)
{
    this->qgs = qgs;
}

void GraphManager::addVertices(std::string filename)
{
    QBrush brush(QColor("black"));

    CrowdParser cp;
    cp.parseFile(filename, simulation);
    for (size_t i = 0; i < simulation.size(); i++)
    {
        vector<Vertex> vertices = simulation.at(i);
        for (size_t j = 0; j < vertices.size(); j++)
        {
            if (circlemap.find(vertices.at(j)) == circlemap.end())
            {
                QGraphicsEllipseItem* item = new QGraphicsEllipseItem(0,0,CIRCLE_DIM, CIRCLE_DIM);
                item->setBrush(brush);
                item->setZValue(2);
                qgs->addItem(item);
                circlemap[vertices.at(j)] = item;
                ltocmap[vertices.at(j).id()] = vertices.at(j);
            }
        }
    }
    qDebug(("Number of vertices: " + to_string(circlemap.size())).c_str());
}

void GraphManager::addEdges(string filename)
{
    QPen pen(QColor("red"));
    EdgeParser ep;
    ep.parseFile(filename, edges);
    for (size_t i = 0; i < edges.size(); i++)
    {
       QGraphicsLineItem* item = new QGraphicsLineItem(0,0,0,0);
       item->setPen(pen);
       qgs->addItem(item);
       linemap[edges.at(i)] = item;
    }
}

void GraphManager::drawTick(unsigned int ticknum)
{
    ticknum = ticknum % simulation.size();
    // TODO Make all vertices invisible.
    vector<Vertex> vertices = simulation.at(ticknum);
    for (int i = 0; i < vertices.size(); i++)
    {
        Vertex v = vertices.at(i);
        // TODO Make vertex v visible.
        circlemap.at(v)->setPos(v.location().x(), v.location().y());
    }
    for (int i = 0; i < edges.size(); i++)
    {
        bool edgePresent = false;
        Edge e = edges.at(i);
        for (int i = 0; i < e.spellStart.size(); i++) {
          int start = e.spellStart[i];
          int stop = e.spellStop[i];
          if ((unsigned int)start <= ticknum && ticknum <= (unsigned int)end) {
            edgePresent = true;
            break;
          }
        }

        if (edgePresent)
        {
          qreal x1, x2, y1, y2;
          x1 = circlemap.at(ltocmap.at(e.source))->pos().x() + (CIRCLE_DIM / 2);
          x2 = circlemap.at(ltocmap.at(e.target))->pos().x() + (CIRCLE_DIM / 2);
          y1 = circlemap.at(ltocmap.at(e.source))->pos().y() + (CIRCLE_DIM / 2);
          y2 = circlemap.at(ltocmap.at(e.target))->pos().y() + (CIRCLE_DIM / 2);
          linemap.at(e)->setLine(x1, y1, x2, y2);
          linemap.at(e)->setVisible(true);
        }
        else
        {
          linemap.at(e)->setVisible(false);
        }
    }
}
