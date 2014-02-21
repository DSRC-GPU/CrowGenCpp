package org.acaro.crowdgenerator.clusterers;

import java.io.IOException;
import java.util.Collection;
import java.util.Map;
import java.util.Map.Entry;

import org.acaro.crowdgenerator.Vertex;
import org.ejml.simple.SimpleMatrix;

import com.google.common.collect.LinkedListMultimap;
import com.google.common.collect.Maps;
import com.google.common.collect.Multimap;

public class RMCLClusterer {
  // inflate exponent
  private static double r = 1.20d;
  // pre-inflate exponent
  private static double p = 8.0d;
  // length of the walks (number of regularizations per iteration)
  private static int length = 1;
  // threshold to consider flow 0
  private static double threshold = 0.1d;
  
  private static double prune = 0.01;
  
  private static int max_iterations = 100;
  
  private static SimpleMatrix adjacency(Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int numvertices) {
    // start from the identity matrix as we want self-loops
    SimpleMatrix adj = SimpleMatrix.identity(numvertices);
    for (Entry<Vertex,Map<Vertex,Integer>> internal: aggregateGraph.entrySet()) {
      int row = internal.getKey().id();
      for (Entry<Vertex, Integer> edge: internal.getValue().entrySet()) {
        int col = edge.getKey().id();
        double weight = (double) edge.getValue() / total;
        // can we do the diagonalization more efficient?
        adj.set(row, col, weight);
        adj.set(col, row, weight);
      }
    }
    return adj;
  }
  
  private static SimpleMatrix normalize(SimpleMatrix adj) {
    int numvertices = adj.numCols();
    SimpleMatrix ddm = new SimpleMatrix(numvertices, numvertices);
    for (int i = 0; i < numvertices; i++) {
      double columnSum = 0;
      for (int j = 0; j < numvertices; j++) {
        double column = adj.get(j, i);
        if (column > 0) {
          columnSum += column;
        }
      }
      ddm.set(i, i, columnSum);
    }
    return adj.mult(ddm.invert());
  }
  
  public static boolean hasConverged(SimpleMatrix m) {
    int totalCount = 0;
    for (int i = 0; i < m.numCols(); i++) {
      int count = 0;
      for (int j = 0; j < m.numRows(); j++) {
        if (m.get(j, i) > threshold) {
          if (++count > 1) {
            return false;
          }
        }
      }
      totalCount++;
    }
    return totalCount == m.numCols();
  }

  public static SimpleMatrix regularize(SimpleMatrix m, SimpleMatrix ctm) {
    return m.mult(ctm);
    //return m.mult(m);
  }
  
  public static SimpleMatrix inflate(SimpleMatrix m, double exp) {
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        m.set(i, j, Math.pow(m.get(i, j), exp));
      }
    }
    return normalize(m);
  }
  
  public static SimpleMatrix prune(SimpleMatrix m, double prune) {
    if (prune == 0) {
      return m;
    }
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        if (m.get(i, j) < prune) {
          m.set(i, j, 0);
        }
      }
    }
    return normalize(m);
  }
  
  public static Multimap<Integer, Integer> extractClusters(SimpleMatrix m) {
    Multimap<Integer,Integer> clusters = LinkedListMultimap.create();
    for (int i = 0; i < m.numCols(); i++) {
      int maxId = -1;
      double maxValue = -1;
      for (int j = 0; j < m.numRows(); j++) {
        double value = m.get(j, i);
        if (value > threshold) {
          if (value > maxValue) {
            maxValue = value;
            maxId = j;
          }
        }
      }
      clusters.put(maxId, i);
    }
    return clusters;
  }
  
  public static Map<Integer,Collection<Integer>> cluster(Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int numvertices) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newTreeMap(); 
    
    // initialize as adjacency matrix with self-loops
    SimpleMatrix adj = adjacency(aggregateGraph, total, numvertices);
    //System.out.println("adjacency computed " + adj.toString());

    // compute canonical transition matrix
    SimpleMatrix ctm = normalize(adj);
    //System.out.println("ctm computed " + ctm.toString());
    
    // main algorithm
    SimpleMatrix m = ctm.copy();
    // pre-inflate
    if (p != 0) {
      m = inflate(m, p);
    }

    int iterations = 0;
    do {
      for (int l = 0; l < length; l++) {
        m = regularize(m, ctm);
        //System.out.println("regularize computed " + m.toString());
      }
      m = inflate(m, r);
      //System.out.println("inflate computed " + m.toString());
      m = prune(m, prune);
    } while (!hasConverged(m) && ++iterations < max_iterations);
    //System.out.println("final matrix " + m.toString());
    
    for (Entry<Integer, Collection<Integer>> entry : extractClusters(m).asMap().entrySet()) {
      clusters.put(entry.getKey(), entry.getValue());
    }
    return clusters;
  }
}
