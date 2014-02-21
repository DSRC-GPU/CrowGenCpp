package org.acaro.crowdgenerator.indexing;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import org.acaro.crowdgenerator.LaneVertex;
import org.acaro.crowdgenerator.Vertex;

public class QuadTree implements SpatialIndex {
  private Node root;

  // helper node data type
  private class Node {
    List<Vertex> v = new LinkedList<Vertex>(); // associated data
    int x, y;              // x- and y- coordinates
    Node NW, NE, SE, SW;   // four subtrees

    Node(int x, int y, Vertex v) {
      this.x = x;
      this.y = y;
      this.v.add(v);
    }
  }

  @Override
  public synchronized Vertex add(Vertex v) {
    root = insert(root, v.x(), v.y(), v);
    return v;
  }

  @Override
  public Iterable<Vertex> getVertices(int x, int y, float radius) {
    int r = (int) Math.ceil(radius);
    List<Vertex> results = new LinkedList<Vertex>();
    Interval2D boundingBox = new Interval2D(
        new Interval(x - r, x + r),
        new Interval(y - r, y + r));
    query2D(boundingBox, results);
    Iterator<Vertex> it = results.iterator();
    while (it.hasNext()) {
      if (distance(it.next(), x, y) > radius) {
        it.remove();
      }
    }
    return results;
  }

  private double distance(Vertex v, int x, int y) {
    return Math.sqrt(Math.pow(v.x() - x, 2) + Math.pow(v.y() - y, 2)); 
  }

  private Node insert(Node h, int x, int y, Vertex v) {
    if (h == null) return new Node(x, y, v);
    if (x == h.x && y == h.y) h.v.add(v);  // duplicate
    else if (x < h.x  && y < h.y)  h.SW = insert(h.SW, x, y, v);
    else if (x < h.x  && y >= h.y) h.NW = insert(h.NW, x, y, v);
    else if (x >= h.x && y < h.y)  h.SE = insert(h.SE, x, y, v);
    else if (x >= h.x && y >= h.y) h.NE = insert(h.NE, x, y, v);
    return h;
  }

  public void query2D(Interval2D rect, List<Vertex> results) {
    query2D(root, rect, results);
  }

  private void query2D(Node h, Interval2D rect, List<Vertex> results) {
    if (h == null) {
      return;
    }
    int xmin = rect.intervalX.low;
    int ymin = rect.intervalY.low;
    int xmax = rect.intervalX.high;
    int ymax = rect.intervalY.high;
    if (rect.contains(h.x, h.y)) {
      results.addAll(h.v);
      
    }
    if (xmin < h.x  && ymin < h.y)  query2D(h.SW, rect, results);
    if (xmin < h.x  && ymax >= h.y) query2D(h.NW, rect, results);
    if (xmax >= h.x && ymin < h.y)  query2D(h.SE, rect, results);
    if (xmax >= h.x && ymax >= h.y) query2D(h.NE, rect, results);
  }

  public static void main(String[] args) {
    QuadTree st = new QuadTree();

    st.add(new LaneVertex(1, 10, 10));
    st.add(new LaneVertex(2, 12, 10));
    st.add(new LaneVertex(3, 30, 30));
    st.add(new LaneVertex(4, 50, 05));
    st.add(new LaneVertex(5, 100, 10));
    st.add(new LaneVertex(6, 10, 100));

    for (Vertex v : st.getVertices(12, 12, 30.0f)) {
      System.out.println(v);
    }
  }
}
