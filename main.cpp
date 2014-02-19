
#include <iostream>

#include "Vertex.hpp"
#include "Crowd.hpp"
#include "MoveMap.hpp"

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

  return 0;
}
