package org.acaro.crowdgenerator.clusterers;

import it.unimi.dsi.fastutil.ints.IntOpenHashSet;

import java.io.IOException;
import java.util.Collection;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.acaro.crowdgenerator.Vertex;
import org.ejml.simple.SimpleMatrix;
import org.jgrapht.UndirectedGraph;
import org.jgrapht.alg.ConnectivityInspector;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.SimpleGraph;

import com.google.common.collect.LinkedListMultimap;
import com.google.common.collect.Maps;
import com.google.common.collect.Multimap;

public class MySpatialClusterer {
  private static double prune = 0.05d;
  private static double threshold = 0.05d;
  private static double inflate = 3.0d;
  private static int length = 3;
  private static int sharpens = 1;
  private static int max_iterations = 1;

  private static double cosineSimilarity(int v1, int v2, SimpleMatrix m) {
    double sum = 0;
    double norm1 = 0, norm2 = 0;
    for (int k = 0; k < m.numCols(); k++) {
      double v1Val = m.get(v1, k);
      double v2Val = m.get(v2, k);
      
      // compute dot product
      if (v1Val != 0 && v2Val != 0) {
        sum += v1Val * v2Val;
      }
      // compute v1 norm
      if (v1Val != 0) {
        norm1 += Math.pow(v1Val, 2);
      }
      // compute v2 norm
      if (v2Val != 0) {
        norm2 += Math.pow(v2Val, 2);
      }
    }
    double similarity = sum / (Math.sqrt(norm1) * Math.sqrt(norm2));
    return similarity;
  }
  
  private static double jaccardSimilarity(int v1, int v2, SimpleMatrix m) {
    IntOpenHashSet h1 = new IntOpenHashSet();
    IntOpenHashSet h2 = new IntOpenHashSet();
    
    for (int k = 0; k < m.numCols(); k++) {
      if (m.get(v1, k) > threshold) {
        h1.add(k);
      }
    }
    
    for (int k = 0; k < m.numCols(); k++) {
      if (m.get(v2, k) > threshold) {
        h2.add(k);
      }
    }

    int sizeV1 = h1.size();
    
    // compute intersections between h1 and h2
    h1.retainAll(h2);
    // compute unique elements 
    h2.removeAll(h1);
    
    int union = sizeV1 + h2.size();
    int intersection = h1.size();
    
    double coefficient = (double) intersection / union;
    return (union > 0) ? coefficient : 0;
  }
  
  private static SimpleMatrix adjacency(
      Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int numvertices) {
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
        double column = adj.get(i, j);
        if (column > 0) {
          columnSum += column;
        }
      }
      ddm.set(i, i, columnSum);
    }
    return adj.mult(ddm.invert());
  }
  
  public static SimpleMatrix square(SimpleMatrix m, SimpleMatrix ctm) {
    //return m.mult(ctm);
    return m.mult(m);
  }
  
  public static SimpleMatrix sharpen(SimpleMatrix m) {
    SimpleMatrix sm = new SimpleMatrix(m.numRows(), m.numCols());
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        if (i != j && m.get(i, j) != 0) {
          //sm.set(i, j, jaccardSimilarity(i, j, m));
          sm.set(i, j, cosineSimilarity(i, j, m));
        }
      }
    }
    return sm;
  }
  
  public static SimpleMatrix prune(SimpleMatrix m, double prune) {
    if (prune == 0) {
      return m;
    }
    
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        if (m.get(i, j) < prune) {
          m.set(i, j, 0.0d);
        }
      }
    }
    return normalize(m);
  }
  
  public static SimpleMatrix inflate(SimpleMatrix m, double exp) {
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        m.set(i, j, Math.pow(m.get(i, j), exp));
      }
    }
    return normalize(m);
  }
  
  private static Multimap<Integer, Integer> extractClusters(SimpleMatrix m) {
    Multimap<Integer,Integer> clusters = LinkedListMultimap.create();
    // convert to graph
    UndirectedGraph<Integer, DefaultEdge> g = 
        new SimpleGraph<Integer, DefaultEdge>(org.jgrapht.graph.DefaultEdge.class);
    for (int i = 0; i < m.numRows(); i++) {
      g.addVertex(i);
    }
    for (int i = 0; i < m.numRows(); i++) {
      for (int j = 0; j < m.numCols(); j++) {
        if (i != j && m.get(i, j) > threshold) {
          g.addEdge(i, j);
        }
      }
    }
    // compute connected components
    ConnectivityInspector<Integer, DefaultEdge> inspector = 
        new ConnectivityInspector<Integer, DefaultEdge>(g);
    int clusterId = 0;
    for (Set<Integer> set : inspector.connectedSets()) {
      clusters.putAll(clusterId++, set);
    }
    return clusters;
  }

  public static Map<Integer,Collection<Integer>> cluster(
      Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int numvertices) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newTreeMap(); 
    
    // initialize as adjacency matrix with self-loops
    SimpleMatrix adj = adjacency(aggregateGraph, total, numvertices);
    //System.out.println("adjacency computed " + adj.toString());

    // compute canonical transition matrix
    //SimpleMatrix ctm = normalize(adj);
    SimpleMatrix ctm = adj;
    //System.out.println("ctm computed " + ctm.toString());
    
    // main algorithm
    SimpleMatrix m = ctm.copy();
    m = normalize(m);
    m = inflate(m, 3.0d);
    int iterations = 0;
    SimpleMatrix totalM = new SimpleMatrix(numvertices, numvertices);
    do {
      for (int l = 0; l < length; l++) {
        //m = normalize(m);
        m = square(m, ctm);
        System.out.println("pre sum" + totalM.toString());
        totalM = totalM.plus(m);
        System.out.println("after sum" + totalM.toString());
        //System.out.println(l + " regularize computed " + totalM.toString());
      }
      System.out.println("random walks computed " + m.toString());
      m = normalize(totalM);
      //m = totalM;
      m = prune(m, prune);
      System.out.println("pruning computed " + m.toString());
      m = inflate(m, inflate);
      System.out.println("inflation computed " + m.toString());
    } while (++iterations < max_iterations);
    //System.out.println("final matrix " + m.toString());
    //m = normalize(m);
    System.out.println(" final computed " + m.toString());
    for (Entry<Integer, Collection<Integer>> entry : extractClusters(m).asMap().entrySet()) {
      clusters.put(entry.getKey(), entry.getValue());
    }
    return clusters;
  }
}
