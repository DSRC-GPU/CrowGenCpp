package org.acaro.crowdgenerator.clusterers;

import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.acaro.crowdgenerator.Edge;
import org.acaro.crowdgenerator.Vertex;
import org.acaro.crowdgenerator.visualizer.EigenSpace;
import org.acaro.spectralclustering.SpectralClustering;
import org.acaro.spectralclustering.KMeansClusterer.Centroid;
import org.acaro.spectralclustering.SpectralClustering.EigenValue;
import org.ejml.simple.SimpleMatrix;

import com.google.common.collect.Maps;
import com.google.common.collect.SortedSetMultimap;

public class NewSpectralClusterer {
  
  private static SimpleMatrix adjacency(
      SortedSetMultimap<Vertex,Edge> aggregateGraph, int numvertices) {
    // start from the identity matrix as we want self-loops
    SimpleMatrix adj = new SimpleMatrix(numvertices, numvertices);
    for (Entry<Vertex,Edge> entry: aggregateGraph.entries()) {
      Edge edge = entry.getValue();
      int row = entry.getKey().id();
      int col = edge.getDst().id();
      double weight = (double) edge.getWeight();
      // can we do the diagonalization more efficient?
      adj.set(row, col, weight);
      adj.set(col, row, weight);
    }
    return adj;
  }
  
  public static Map<Integer,Collection<Integer>> cluster(
      SortedSetMultimap<Vertex,Edge> aggregateGraph, int numvertices, double[] values, EigenSpace eispace) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newTreeMap();
    SimpleMatrix W = adjacency(aggregateGraph, numvertices);
    
    SpectralClustering clusterer = new SpectralClustering(W, 4);
    List<Centroid> centroids = clusterer.cluster();
    List<EigenValue> eigenValues = clusterer.getEigenValues();
    int i = 0;
    for (EigenValue eiv : eigenValues) {
      values[i++] = eiv.getValue();
    }
    Map<Integer, Collection<Integer>> internalClusters = Maps.newTreeMap(); 
    for (Centroid centroid : centroids) {
      if (centroid.getDataPoints().size() > 0) {
        internalClusters.put(Collections.max(centroid.getDataPoints()), centroid.getDataPoints());
      }
    }
    int c = 0;
    for (Entry<Integer, Collection<Integer>> e : internalClusters.entrySet()) {
      clusters.put(c++, e.getValue());
    }
    
    SimpleMatrix eiM = clusterer.getEiMatrix();
    //System.out.println("EiM " + eiM.numRows() + ":" + eiM.numCols());
    double[][] eiMPoints = new double[eiM.numRows()][2];
    for (int k = 0; k < eiM.numRows(); k++) {
      double[] eiMPoint = new double[2];
      eiMPoint[0] = eiM.get(k, 0);
      eiMPoint[1] = eiM.get(k, 1);
      //System.out.println("EiM point " + Arrays.toString(eiMPoint));
      eiMPoints[k] = eiMPoint;
    }
    eispace.draw(eiMPoints);
    
    return clusters;
  }
}
