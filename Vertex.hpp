
#ifndef VERTEX_HPP
#define VERTEX_HPP

class Vertex
{
  private:
    int id;
    int xpos, ypos;

  public:
    Vertex(int);
    int x() const;
    int x(int newx);
    int y() const;
    int y(int newy);
    bool equal(Vertex other) const;
};

#endif
