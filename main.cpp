
#include <iostream>

#include "Vertex.hpp"
#include "Crowd.hpp"
#include "MoveMap.hpp"
#include "CrowdGenerator.hpp"

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

  cout << "MoveMap test" << endl;

  MoveMap mm(6000,10,100);

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

  return 0;
}
