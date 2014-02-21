package org.acaro.crowdgenerator;

import java.util.Random;

import org.acaro.crowdgenerator.indexing.SpatialIndex;

import com.google.common.collect.SortedSetMultimap;

public class ProximityGraphGenerator implements Runnable {
  private static final int TX_RANGE_AVG = 60;
  private static final int TX_RANGE_DEV = 0;
  private static final float PR_LINK_FAIL = 0.0f;
  private SortedSetMultimap<Vertex,Edge> edges;
  private SpatialIndex index;
  private Vertex[] vertices;
  private int partition;
  private int partitions;

  public ProximityGraphGenerator(int partition, int partitions, Vertex[] vertices,  
      SortedSetMultimap<Vertex,Edge> edges, SpatialIndex index) {
    this.vertices = vertices;
    this.partition = partition;
    this.partitions = partitions;
    this.edges = edges;
    this.index = index;
  }

  @Override
  public void run() {
    Random rn = new Random();
    for (int i = partition; i < vertices.length; i += partitions) {
      Vertex v1 = vertices[i];
      for (Vertex v2 : index.getVertices(v1.x(), v1.y(), 200)) {
        if (distance(v1, v2) < TX_RANGE_AVG + (float) rn.nextGaussian() * TX_RANGE_DEV && 
            PR_LINK_FAIL < rn.nextFloat() && v1 != v2) {
          if (v1.id() < v2.id()) {
            edges.put(v1, new Edge(v2, 1.0));
          } else {
            edges.put(v2, new Edge(v1, 1.0));
          }
        }
      }
    }	
  }

  private float distance(Vertex v1, Vertex v2) {
    return (float) Math.sqrt(Math.pow(v1.x() - v2.x(), 2) + Math.pow(v1.y() - v2.y(), 2));
  }
}
