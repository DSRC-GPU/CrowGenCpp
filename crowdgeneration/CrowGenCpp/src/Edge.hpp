
#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>
#include <vector>
#include <utility>
#include "Vertex.hpp"

// This class represents an Edge between to Vertex objects in the gexf proximity
// graph.
class Edge
{
  private:
    Vertex& _source, _target;
    std::vector<std::pair<unsigned int, unsigned int>> _lifetimes;
    std::string _id;
    std::string _type;

  public:
    Edge(Vertex&, Vertex&);
    Edge(const Edge&);

    std::string id() const;
    Vertex source() const;
    Vertex target() const;
    std::vector<std::pair<unsigned int, unsigned int>>& lifetimes();
    std::vector<std::pair<unsigned int, unsigned int>>::iterator
     getLifetimeWithEnd(unsigned int end);
    std::string type() const;
    void type(std::string);

    bool operator<(const Edge&) const;
    bool operator==(const Edge&) const;
    Edge& operator=(const Edge&);
};

namespace std {
  template<>
  class hash<Edge>
  {
    public:
      long operator() (const Edge& e) const
      {
        return (e.source().id() << 16) ^ e.target().id();
      }
  };
}

#endif

