
#include <iostream>

#include "Crowd.hpp"
#include "CrowdGenerator.hpp"
#include "GroupDescriptor.hpp"
#include "MoveSimulator.hpp"
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

  cout << "GroupDescriptor test" << endl;

  GroupDescriptor mm(60,10,100);

  cout << mm.population() << endl;
  cout << mm.width() << endl;
  cout << mm.height() << endl;

  cout << "CrowdGenerator test" << endl;

  CrowdGenerator cg; 

  cout << c.size() << endl;
  cg.populate(c, mm);
  cout << c.size() << endl;

  vector<Vertex> vect = c.getVertices();
  for (size_t i = 0; i < vect.size(); i++)
  {
    cout << vect.at(i).x() << endl;
    cout << vect.at(i).y() << endl;
    cout << "---" << endl;
  }

  cout << "MoveSimulator test" << endl;

  MoveSimulator ms;

  ms.doTick(c, mm);

  return 0;
}
