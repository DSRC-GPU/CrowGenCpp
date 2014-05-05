#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
    int source, target, start, end;
    Edge();
    bool operator<(const Edge& e) const;
};

#endif // EDGE_H
