#ifndef EDGE_H
#define EDGE_H

#include <vector>

class Edge
{
public:
    int source, target;
    std::vector<int> spellStart;
    std::vector<int> spellStop;
    Edge();
    bool operator<(const Edge& e) const;
};

#endif // EDGE_H
