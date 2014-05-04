
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
    unsigned int _detectionRange = 50;
    unsigned int _fieldWidth, _fieldHeight;
    
    unsigned int _wsquares, _hsquares;
    vector<Vertex*>** _squares;

    double _falseNeg, _falsePos;
    vector<vector<Vertex>> simulationrun;
    vector<Vertex> vertices;
    vector<Edge> edges;

    void graphUpdate(unsigned int);
    void updateEdge(vector<Edge>&, Vertex&, Vertex&, int);
    void updateVertex(vector<Vertex>&, Vertex, int);

    bool falsePositive() const;
    bool falseNegative() const;
    bool falseData(double) const;

  public:
    ProximityGraphGenerator();
    
    // Set the detection range between two vertices to create an edge
    void setDetectionRange(int);

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

    // Creates a SVG visualization.
    void createVisualization();
    
    // Write graph data to some output.
    void writeGraph(string);
};


#endif

