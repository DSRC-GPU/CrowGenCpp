package org.acaro.crowdgenerator.clusterers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.acaro.crowdgenerator.Vertex;

import net.sf.javaml.clustering.mcl.MarkovClustering;
import net.sf.javaml.clustering.mcl.SparseMatrix;

import com.google.common.collect.Maps;
import com.google.common.collect.SortedSetMultimap;

public class Clusterer {
  private SparseMatrix matrix;
  private double maxResidual = 0.001;
  private double pGamma = 2.0;
  private double loopGain = 0.;
  private double maxZero = 0.001;

  public Clusterer(SparseMatrix matrix) {
    this.matrix = matrix;
  }

  public Clusterer(SparseMatrix matrix, double maxResidual, double pGamma, double loopGain, double maxZero) {
    this(matrix);
    this.maxResidual = maxResidual;
    this.pGamma = pGamma;
    this.loopGain = loopGain;
    this.maxZero = maxZero;
  }

  public static SparseMatrix fromTimelineToMatrix(SortedSetMultimap<Vertex, Vertex>[] graphs, int start, int end, int numberOfVertices) {
    SparseMatrix matrix = new SparseMatrix(numberOfVertices, numberOfVertices);
    for (int i = start; i < end; i++) {
      SortedSetMultimap<Vertex, Vertex> graph = graphs[i];
      for (Entry<Vertex, Vertex> edge : graph.entries()) {
        int src = edge.getKey().id();
        int dst = edge.getValue().id();
        double val = matrix.get(src, dst);
        matrix.set(src, dst, val + 1);
      }
    }
    return matrix;
  }

  public Map<Vertex, Integer> cluster() {
    Map<Vertex, Integer> results = Maps.newHashMap();
    MarkovClustering mcl = new MarkovClustering();
    SparseMatrix resultMatrix = mcl.run(matrix, maxResidual, pGamma, loopGain, maxZero);

    Map<Integer, ArrayList<Integer>> clusters = new HashMap<Integer, ArrayList<Integer>>();
    int clusterCount = 0;
    double[][] mat = matrix.getDense();
    for (int i = 0; i < mat.length; i++) {
      for (int j = 0; j < mat[0].length; j++) {
        double value = mat[i][j];
        if (value != 0.0) {
          if (i == j) {
            continue;
          }
          if (clusters.containsKey(j)) {
          // Already seen "column" -- get the cluster and add column
          ArrayList<Integer> columnCluster = clusters.get(j);
          if (clusters.containsKey(i)) {
            // We've already seen row also -- join them
            ArrayList<Integer> rowCluster = clusters.get(i);
            if (rowCluster == columnCluster) {
              continue;
            }
            columnCluster.addAll(rowCluster);
                                          clusterCount--;
                                      } else {
                                            // debugln("Adding "+row+" to "+columnCluster.getClusterNumber());
                                            columnCluster.add(i);
                                        }
                                    for (Integer in : columnCluster) {
                                          clusters.put(in, columnCluster);
                                      }
                                } else {
                                      ArrayList<Integer> rowCluster;
                                      // First time we've seen "column" -- have we already seen "row"
                                      if (clusters.containsKey(i)) {
                                            // Yes, just add column to row's cluster
                                            rowCluster = clusters.get(i);
                                            // debugln("Adding "+column+" to "+rowCluster.getClusterNumber());
                                            rowCluster.add(j);
                                        } else {
                                              rowCluster = new ArrayList<Integer>();
                                              clusterCount++;
                                              // debugln("Created new cluster "+rowCluster.getClusterNumber()+" with "+row+" and "+column);
                                              rowCluster.add(j);
                                              rowCluster.add(i);
                                          }
                                      for (Integer in : rowCluster) {
                                            clusters.put(in, rowCluster);
                                        }
                                  }
                          }
                        if (cancelled) {
                              return clusters;
                          }
                    }
              }
     
            return clusters;
        }
}
}
