
#ifndef VERTEX_HPP
#define VERTEX_HPP

// This class represents a vertex in a gexf proximity graph.
class Vertex
{
  private:
    int _id, _label;
    int _start, _end;

  public:
    Vertex();
    Vertex(int, int);

    int id() const;
    int id(int);
    int label() const;
    int label(int);
    int start() const;
    int start(int);
    int end() const;
    int end(int);
};

#endif

