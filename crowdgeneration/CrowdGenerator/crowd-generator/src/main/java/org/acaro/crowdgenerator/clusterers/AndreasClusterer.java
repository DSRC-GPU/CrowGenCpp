package org.acaro.crowdgenerator.clusterers;

import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Random;
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

public class AndreasClusterer {
  private static double stability_threshold = 0.0001d;
  private static double phi = 0.9;
  
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
    SimpleMatrix ddm = new SimpleMatrix(adj.numRows(), adj.numCols());
    for (int i = 0; i < adj.numRows(); i++) {
      double columnSum = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double column = adj.get(i, j);
        if (column > 0) {
          columnSum += column;
        }
      }
      for (int j = 0; j < adj.numCols(); j++) {
        double column = adj.get(i, j);
        if (column > 0) {
          ddm.set(i, j, column / columnSum);
        }
      }
    }
    return ddm;
  }
  
  private static SimpleMatrix computeLaplacian(SimpleMatrix adj) {
    SimpleMatrix degreeMatrix = new SimpleMatrix(adj.numRows(), adj.numCols());
    for (int i = 0; i < adj.numRows(); i++) {
      double sum = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double val = adj.get(i, j);
        if (val != 0) {
          sum += val;
        }
      }
      degreeMatrix.set(i, i, sum);
    }
    return degreeMatrix.minus(adj);
  }
  
  private static double[] computeEdges(SimpleMatrix laplacian, double[] vector) {
    double[] results = new double[vector.length];
    for (int i = 0; i < laplacian.numRows(); i++) {
      for (int j = 0; j < laplacian.numCols(); j++) {
        double value = laplacian.get(i, j);
        if (value != 0 && vector[j] != 0) {
          results[i] += value * vector[j];
        }
      }
    }
    return results;
  }
  
  public static int sign(double f) {
    if (f != f) throw new IllegalArgumentException("NaN");
    if (f == 0) return 0;
    f *= Double.POSITIVE_INFINITY;
    if (f == Double.POSITIVE_INFINITY) return +1;
    if (f == Double.NEGATIVE_INFINITY) return -1;

    throw new IllegalArgumentException("Unfathomed double");
  }
  
  private static SimpleMatrix computeCuts(SimpleMatrix adj, double[] edges) {
    SimpleMatrix cutMatrix = new SimpleMatrix(adj.numRows(), adj.numCols());
    for (int i = 0; i < adj.numRows(); i++) {
      for (int j = 0; j < adj.numCols(); j++) {
        if (adj.get(i, j) > 0) {
          if ((edges[i] <= 0 && edges[j] <= 0) || (edges[i] >= 0 && edges[j] >= 0)) {
            cutMatrix.set(i, j, 1.0);
          }
        }
      }
    }
    return cutMatrix;
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
        if (i != j && m.get(i, j) > 0) {
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
  
  private static boolean isStable(double[] vec1, double[] vec2, int iterations) {
    double v1norm = 0;
    for (int i = 0; i < vec1.length; i++) {
      v1norm += Math.pow(vec1[i], 2);
    }
    double v2norm = 0;
    for (int i = 0; i < vec2.length; i++) {
      v2norm += Math.pow(vec2[i], 2);
    }
    
    return Math.abs(Math.sqrt(v1norm) - Math.sqrt(v2norm)) < stability_threshold;
  }
  
  public static double[] initializeRandomValues(double[] vector) {
    Random rnd = new Random();
    for (int i = 0; i < vector.length; i++) {
      vector[i] = rnd.nextDouble();
    }
    return vector;
  }
  
  public static double[] initializeStabilityValues(double[] vector, SimpleMatrix adj) {
    for (int i = 0; i < adj.numRows(); i++) {
      int numberOfNeighbors = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double weight = adj.get(i, j);
        if (weight == 0) {
          continue;
        }
        numberOfNeighbors++;
      }
      double avg  = 1.0d / numberOfNeighbors;
      double RMSE = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double weight = adj.get(i, j);
        if (weight == 0) {
          continue;
        }
        RMSE += Math.pow(weight - avg, 2);
      }
      vector[i] = 1 - Math.sqrt(RMSE / numberOfNeighbors);
    }
    return vector;
  }
  
  public static void updateValues(double[] oldValues, double[] newValues, SimpleMatrix adj) {
    for (int i = 0; i < adj.numRows(); i++) {
      double newValue = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double weight = adj.get(i, j);
        if (weight == 0) {
          continue;
        }
        newValue += weight * oldValues[j];
      }
      newValues[i] = phi * oldValues[i] + (1 - phi) * newValue;
    }
  }

  public static Map<Integer,Collection<Integer>> cluster(
      Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int numvertices) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newTreeMap();
    double[] oldValues = new double[numvertices];
    double[] newValues = new double[numvertices];
    
    // initialize as adjacency matrix with self-loops
    SimpleMatrix adj = adjacency(aggregateGraph, total, numvertices);
    adj = normalize(adj);
    //System.out.println("matrix=" + adj.toString());

    initializeStabilityValues(newValues, adj);
    System.out.println("stability values=" + Arrays.toString(newValues));
    
    int iterations = 0;
    do {
      System.arraycopy(newValues, 0, oldValues, 0, newValues.length);
      updateValues(oldValues, newValues, adj);
      iterations++;
    } while (!isStable(oldValues, newValues, iterations));
    System.out.println(iterations + " values: " + Arrays.toString(newValues));

    SimpleMatrix cutMatrix = computeCuts(adj, computeEdges(computeLaplacian(adj), newValues));
    for (Entry<Integer, Collection<Integer>> entry : extractClusters(cutMatrix).asMap().entrySet()) {
      clusters.put(entry.getKey(), entry.getValue());
    }
    return clusters;
  }
}
