#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <QGraphicsScene>
#include <QGraphicsLineItem>

#include "../../crowdgeneration/CrowGenCpp/src/Vertex.hpp"

class GraphManager
{
private:
    QGraphicsScene* qgs;
    std::vector<std::vector<Vertex>> simulation;
    std::map<Vertex,QGraphicsLineItem*> linemap;

public:
    GraphManager();
    void init(QGraphicsScene* qgs);
    void addVertices(std::string filename);
    void addEdges(std::string filename);
    void drawTick(unsigned int ticknum);
};

#endif // GRAPHMANAGER_H
