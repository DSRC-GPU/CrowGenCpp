#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <QGraphicsScene>
#include <QGraphicsLineItem>

#include "../../crowdgeneration/CrowGenCpp/src/Vertex.hpp"
#include "edge.h"

class GraphManager
{
private:
    QGraphicsScene* qgs;
    std::vector<std::vector<Vertex>> simulation;
    std::vector<Edge> edges;
    std::map<Vertex,QGraphicsEllipseItem*> circlemap;
    std::map<Edge,QGraphicsLineItem*> linemap;
    std::map<int, Vertex> ltocmap;

public:
    GraphManager();
    void init(QGraphicsScene* qgs);
    void addVertices(std::string filename);
    void addEdges(std::string filename);
    void drawTick(unsigned int ticknum);
};

#endif // GRAPHMANAGER_H
