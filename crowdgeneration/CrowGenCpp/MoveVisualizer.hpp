
#ifndef MOVEVISUALIZER_HPP
#define MOVEVISUALIZER_HPP

#include <string>

using namespace std;

// The move visualizer parses a xml movement file and creates an animated svg
// that visualizes the movement of all nodes.
class MoveVisualizer
{
  public:
    MoveVisualizer();
    void parse(string);
    void visualize(string);
    void visualize(string, string);
};

#endif
