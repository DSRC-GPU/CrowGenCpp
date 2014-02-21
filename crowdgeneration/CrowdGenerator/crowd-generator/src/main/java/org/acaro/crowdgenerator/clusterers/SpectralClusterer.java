package org.acaro.crowdgenerator.clusterers;

import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Map;
import java.util.Map.Entry;

import org.acaro.crowdgenerator.Edge;
import org.acaro.crowdgenerator.Vertex;
import org.ejml.data.DenseMatrix64F;
import org.ejml.factory.DecompositionFactory;
import org.ejml.factory.EigenDecomposition;
import org.ejml.simple.SimpleMatrix;
import org.ejml.simple.SimpleSVD;

import com.google.common.collect.Maps;
import com.google.common.collect.SortedSetMultimap;

public class SpectralClusterer {

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
  
  private static SimpleMatrix degree(SimpleMatrix adj) {
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
    return degreeMatrix;
  }
  
  private static SimpleMatrix invertRootDegree(SimpleMatrix adj) {
    SimpleMatrix degreeMatrix = new SimpleMatrix(adj.numRows(), adj.numCols());
    for (int i = 0; i < adj.numRows(); i++) {
      double sum = 0;
      for (int j = 0; j < adj.numCols(); j++) {
        double val = adj.get(i, j);
        if (val != 0) {
          sum += val;
        }
      }
      degreeMatrix.set(i, i, 1 / Math.sqrt(sum));
    }
    return degreeMatrix;
  }

  
  public static Map<Integer,Collection<Integer>> cluster(
      SortedSetMultimap<Vertex,Edge> aggregateGraph, int numvertices, double[] values) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newTreeMap();
    
    //SimpleMatrix I = SimpleMatrix.identity(numvertices);
    //SimpleMatrix W = adjacency(aggregateGraph, numvertices);
    //SimpleMatrix D = degree(W);
    // L_{rw} normalized Laplacian
    //SimpleMatrix L = I.minus(D.invert().mult(W));

    SimpleMatrix W = adjacency(aggregateGraph, numvertices);
    SimpleMatrix D = invertRootDegree(W);
    SimpleMatrix L = D.mult(W).mult(D);
    
/*    SimpleSVD svd = L.svd();
    SimpleMatrix eigenvalues = svd.getW().extractDiag();
    for (int i = 0; i < values.length; i++) {
      values[i] = eigenvalues.get(i);
    }*/
    
    EigenDecomposition<DenseMatrix64F> decomp = DecompositionFactory.eig(L.numRows(), true);
    decomp.decompose(L.getMatrix());
    for (int i = 0; i < decomp.getNumberOfEigenvalues(); i++) {
      if (decomp.getEigenvalue(i).isReal() != true) {
        throw new IllegalStateException("complex eigenvalue");
      }
      values[i] = decomp.getEigenvalue(i).getReal();
    }
    
    //System.out.println("eigenvalues: " + Arrays.toString(values));
    
    return clusters;
  }
}
