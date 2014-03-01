
#ifndef PROXIMITYGRAPHGENERATOR_HPP
#define PROXIMITYGRAPHGENERATOR_HPP

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"

using namespace std;

// The ProximityGraphGenerator class generates a proximity graph based on a
// xml-crowd file. It also allows the user to tweak some probability parameters
// regarding false positive and false negative detection rates.
class ProximityGraphGenerator
{
  private:
    double _falseNeg, _falsePos;
    vector<vector<Vertex>>* simulationrun;
    vector<Vertex>* vertices;
    vector<Edge>* edges;

    void graphUpdate(int);
    void updateEdge(Vertex&, Vertex&, int);
    void updateVertex(Vertex, int);

  public:
    ProximityGraphGenerator();

    // Set the rate at which a node experiences a false negative (should detect
    // an edge, but doesn't). Range = [0,1]
    void setFalseNegative(double);

    // Set the rate at which a node experiences a false positive (shouldn't
    // detect an edge, but does, with a random other edge). Note: you probably
    // want to keep this number very small. Range = [0,1]
    void setFalsePositive(double);

    // Read the xml-crowd file and puts everything in memory.
    void parseCrowd(string);

    // Create a proximity graph based on the parsed xml-crowd file.
    void createGraph();

    // Write graph data to some output.
    void writeGraph() const;
};


#endif

