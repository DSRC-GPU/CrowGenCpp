package org.acaro.crowdgenerator;

public class Edge implements Comparable<Edge> {
  private Vertex dst;
  private double weight;
  
  public Edge(Vertex dst, double weight) {
    this.dst = dst;
    this.weight = weight;
  }
  
  public Vertex getDst() {
    return dst;
  }
  
  public double getWeight() {
    return weight;
  }
  
  @Override
  public String toString() {
    return "v:" + dst + " w:" + weight;
  }

  @Override
  public int compareTo(Edge other) {
    return this.dst.compareTo(other.dst);
  }
  
  @Override
  public boolean equals(Object other) {
      boolean result = false;
      if (other instanceof Edge) {
          Edge that = (Edge) other;
          result = this.dst.equals(that.dst);
      }
      return result;
  }
  
    @Override 
    public int hashCode() {
        return (41 * (41 + this.dst.id()));
    }
}
