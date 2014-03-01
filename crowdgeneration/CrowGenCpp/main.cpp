
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

int main(int argc, char* argv[])
{
  string input = "testmap.xml";
  int numTicks = 100;

  // Parsing inputs.
  for(size_t i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-t"))
    {
      numTicks = atoi(argv[++i]);
    }
    else if (!strcmp(argv[i], "-m"))
    {
      input = argv[++i];
    }
  }

  MapParser parser(input);
  vector<GroupDescriptor> descriptors;
  parser.parse(descriptors);


  MoveSimulator ms;
  Crowd c;

  ms.writeToFile(true);

  cout << "Initializing." << endl;

  ms.initialize(c, descriptors);

  cout << "Simulating...";

  ms.doTick(c, descriptors, numTicks);

  cout << "done!" << endl;

  return 0;
}
