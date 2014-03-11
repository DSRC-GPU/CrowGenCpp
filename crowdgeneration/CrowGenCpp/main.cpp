
#include <iostream>
#include <vector>
#include <string>

#include "Crowd.hpp"
#include "GroupDescriptor.hpp"
#include "MoveSimulator.hpp"
#include "MapParser.hpp"
#include "ProximityGraphGenerator.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  unsigned int u_argc = argc;

  string map_description_file = "";
  string simulation_run_file = "default_sim_run.out";
  int numTicks = 100;
  bool _makeGraph = false;

  // Parsing inputs.
  for (size_t i = 1; i < u_argc; i++)
  {
    if (!strcmp(argv[i], "-t"))
    {
      numTicks = atoi(argv[++i]);
    }
    else if (!strcmp(argv[i], "-m"))
    {
      map_description_file = argv[++i];
    }
    else if (!strcmp(argv[i], "-g"))
    {
      _makeGraph = true;
    }
    else if (!strcmp(argv[i], "-m"))
    {
      map_description_file = argv[++i];
    }
    else if (!strcmp(argv[i], "-s"))
    {
      simulation_run_file = argv[++i];
    }
  }

  // Checking inputs
  if (map_description_file.compare("") == 0)
  {
    cout << "No input map specified. Use the -m parameter." << endl;
    exit(EXIT_FAILURE);
  }

  MapParser parser(map_description_file);
  vector<GroupDescriptor> descriptors;
  parser.parse(descriptors);


  MoveSimulator ms;
  Crowd c;

  ms.writeToFile(true);

  cout << "Initializing." << endl;

  ms.initialize(c, descriptors);

  cout << "Simulating..." << flush;

  ms.doTick(c, descriptors, numTicks);

  cout << "done!" << endl;

  if (_makeGraph)
  {
    cout << "Creating gexf graph..." << flush;

    // We process movement xml to gexf graph.
    ProximityGraphGenerator pgg;
    pgg.parseCrowd(simulation_run_file);
    pgg.createGraph();
    pgg.writeGraph();

    cout << "done!" << endl;
  }

  return 0;
}
