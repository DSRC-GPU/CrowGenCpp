
#include <iostream>

#include "Vertex.hpp"
#include "Crowd.hpp"

using namespace std;

int main()
{
  cout << "Vertex test" << endl;

  Vertex v(0);
  Crowd c;
  cout << v.x() << endl;
  v.x(5);
  cout << v.x() << endl;

  cout << "Crowd test" << endl;

  cout << c.size() << endl;
  c.add(v);
  cout << c.size() << endl;
  c.remove(v);
  cout << c.size() << endl;
  return 0;
}
