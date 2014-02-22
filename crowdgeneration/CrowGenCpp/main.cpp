
#include <iostream>
#include <vector>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "MoveSimulator.hpp"
#include "MapParser.hpp"
#include "Vertex.hpp"

using namespace std;

int main()
{
  cout << "Vertex test" << endl;

  Vertex v(0);
  
  cout << v.x() << endl;
  v.x(5);
  cout << v.x() << endl;

  cout << "Crowd test" << endl;

  Crowd c;

  cout << c.size() << endl;
  c.add(v);
  cout << c.size() << endl;
  c.remove(v);
  cout << c.size() << endl;

  cout << "MapParser test" << endl;

  MapParser parser("testmap.xml");
  vector<GroupDescriptor> descriptors;
  cout << descriptors.size() << endl;
  parser.parse(descriptors);
  cout << descriptors.size() << endl;

  cout << descriptors.at(0).charmap();

  cout << "CrowdGenerator test" << endl;

  CrowdGenerator cg; 

  cout << c.size() << endl;
  cg.populate(c, descriptors);
  cout << c.size() << endl;

  cout << "MoveSimulator test" << endl;

  MoveSimulator ms;

  //ms.doTick(c, mm);

  return 0;
}
