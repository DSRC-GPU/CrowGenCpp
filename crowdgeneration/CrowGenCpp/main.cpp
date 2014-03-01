
#include <iostream>
#include <vector>
#include <string>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "MoveSimulator.hpp"
#include "MapParser.hpp"
#include "Vertex.hpp"

using namespace std;

int main()
{
  string input = "testmap.xml";

  MapParser parser(input);
  vector<GroupDescriptor> descriptors;
  parser.parse(descriptors);

  cout << "MoveSimulator test" << endl;

  MoveSimulator ms;
  Crowd c;

  ms.writeToFile(true);
  ms.initialize(c, descriptors);
  ms.doTick(c, descriptors, 100);

  return 0;
}
