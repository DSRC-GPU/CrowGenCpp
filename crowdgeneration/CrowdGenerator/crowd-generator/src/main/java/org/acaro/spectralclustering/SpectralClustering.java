/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.acaro.spectralclustering;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import org.acaro.spectralclustering.KMeansClusterer.Centroid;
import org.ejml.data.DenseMatrix64F;
import org.ejml.factory.DecompositionFactory;
import org.ejml.factory.EigenDecomposition;
import org.ejml.simple.SimpleMatrix;

import com.google.common.base.Preconditions;
import com.google.common.collect.Lists;

public class SpectralClustering {
  private static final int TRIES = 10;
  private SimpleMatrix A = null;
  private SimpleMatrix EiM = null;
  private List<EigenValue> EiV = null;
  private int K = 0;
  
  private List<Centroid> kMeansCluster(SimpleMatrix data, int k) {
    double minError = Double.POSITIVE_INFINITY;
    List<Centroid> bestSolution = null;
    KMeansClusterer clusterer = new KMeansClusterer(data);
    for (int i = 0; i < TRIES; i++) {
      List<Centroid> centroids = clusterer.chooseRandomCentroids(k);
      List<Centroid> results = clusterer.cluster(centroids);
      double error = clusterer.computeError(results);
      if (error < minError) {
        minError = error;
        bestSolution = results;
      }
    }
    return bestSolution;
  }

  private SimpleMatrix renormalize(SimpleMatrix X) {
    SimpleMatrix Y = new SimpleMatrix(X.numRows(), X.numCols());
    for (int i = 0; i < X.numRows(); i++) {
      double sum = 0;
      for (int j = 0; j < X.numCols(); j++) {
        sum += Math.pow(X.get(i, j), 2);
      }
      if (sum > 0) {
        sum = Math.sqrt(sum);
        for (int j = 0; j < X.numCols(); j++) {
          Y.set(i, j, X.get(i, j) / sum);
        }
      } else {
        for (int j = 0; j < X.numCols(); j++) {
          Y.set(i, j, 0);
        }
      }
    }
    return Y;
  }
  
  private SimpleMatrix computeEigenVectorsMatrix(EigenDecomposition<DenseMatrix64F> decomp, 
      List<EigenValue> eigenValues, int k) {
    SimpleMatrix eigenVectorsMatrix = new SimpleMatrix(eigenValues.size(), k);
    for (int i = 0; i < k; i++) {
      EigenValue eiValue = eigenValues.get(i);
      DenseMatrix64F eiVector = decomp.getEigenVector(eiValue.getLambda());
      for (int j = 0; j < eiVector.numRows; j++) {
        eigenVectorsMatrix.set(j, i, eiVector.get(j, 0));
      }
    }
    return eigenVectorsMatrix;
  }
  
  private int computeEigenGap(List<EigenValue> eigenValues) {
    Preconditions.checkArgument(eigenValues.size() > 0);
    if (eigenValues.size() == 1) {
      return 0;
    }
    int k = 0;
    double maxDiff = Double.NEGATIVE_INFINITY;
    for (int i = 1; i < eigenValues.size(); i++) {
      double diff = Math.abs(eigenValues.get(i).getValue() - eigenValues.get(i - 1).getValue());
      if (diff > maxDiff) {
        k = i;
        maxDiff = diff;
      }
    }
    return k;
  }
  
  private double distance(double[] a, double[] b, double sigma) {
    Preconditions.checkArgument(a.length == b.length, "distance: data points must have same size");
    double value = 0;
    for (int i = 0; i < a.length; i++) {
      if (a[i] != 0 && b[i] != 0) {
        value += Math.pow(a[i] - b[i], 2);
      }
    }
    // Gaussian similarity function
    return Math.exp(- value / (2 * Math.pow(sigma, 2)));
  }
  
  private SimpleMatrix invertRootDegree(SimpleMatrix adj) {
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
  
  private SimpleMatrix initA(double[][] dataset) {
    int numberOfPoints = dataset.length;
    SimpleMatrix A = new SimpleMatrix(numberOfPoints, numberOfPoints);
    for (int i = 0; i < numberOfPoints - 1; i++) {
      for (int j = i + 1; j < numberOfPoints; j++) {
        double value = distance(dataset[i], dataset[j], 30.0);
        A.set(i, j, value);
        A.set(j, i, value);
      }
    }
    return A;
  }
  
  public SpectralClustering(SimpleMatrix A) {
    this.A = A;
  }
  
  public SpectralClustering(SimpleMatrix A, int K) {
    this(A);
    this.K = K;
  }
  
  public SpectralClustering(double[][] dataset) {
    this.A = initA(dataset);
  }
  
  public SpectralClustering(double[][] dataset, int K) {
    this(dataset);
    this.K = K;
  }
  
  public List<EigenValue> getEigenValues() {
    if (EiV == null) {
      throw new IllegalStateException("EiV not initialized, cluster first.");
    }
    return EiV;
  }
  
  public SimpleMatrix getEiMatrix() {
    if (EiM == null) {
      throw new IllegalStateException("EiM not initialized, cluster first.");
    }
    return EiM;
  }
  
  public List<Centroid> cluster() {
    // compute D^{-0.5}
    SimpleMatrix D = invertRootDegree(A);
    // compute D^{-0.5} A D^{-0.5}
    SimpleMatrix L = D.mult(A).mult(D);
    
    EigenDecomposition<DenseMatrix64F> decomp = DecompositionFactory.eig(A.numRows(), true);
    decomp.decompose(L.getMatrix());
    List<EigenValue> eigenValues = Lists.newArrayListWithCapacity(decomp.getNumberOfEigenvalues());
    for (int i = 0; i < decomp.getNumberOfEigenvalues(); i++) {
      if (decomp.getEigenvalue(i).isReal() != true) {
        throw new IllegalStateException("complex eigenvalue");
      }
      eigenValues.add(new EigenValue(i, decomp.getEigenvalue(i).getReal()));
    }
    Collections.sort(eigenValues);
    if (K == 0) {
      K = computeEigenGap(eigenValues);
    }
    EiV = eigenValues;
    SimpleMatrix X = computeEigenVectorsMatrix(decomp, eigenValues, K);
    SimpleMatrix Y = renormalize(X);
    EiM = Y;
    List<Centroid> clusters = kMeansCluster(Y, K);
    return clusters;
  }
  
  public static void main(String[] args) {
    double[][] data = {
        { 1, 1},
        { 1, 2},
        { 1, 3},
        { 2, 1},
        { 2, 2},
        { 2, 3},
        { 3, 1},
        { 3, 2},
        { 3, 3},
        { 11, 11},
        { 11, 12},
        { 11, 13},
        { 12, 11},
        { 12, 12},
        { 12, 13},
        { 13, 11},
        { 13, 12},
        { 13, 13}
    };
    
    SpectralClustering clusterer = new SpectralClustering(data, 2);
    List<Centroid> clusters = clusterer.cluster();
    System.out.println("Found " + clusters.size() + " clusters.");
    int clusterNumber = 0;
    for (Centroid cluster : clusters) {
      System.out.println("Cluster " + ++clusterNumber);
      for (Integer p : cluster.getDataPoints()) {
        System.out.println(Arrays.toString(data[p]));
      }
    }
  }
  
  public class EigenValue implements Comparable<EigenValue> {
    private final int lambda;
    private final double value;
    
    public EigenValue(int lambda, double value) {
      this.lambda = lambda;
      this.value = value;
    }
    
    public int getLambda() {
      return this.lambda;
    }
    
    public double getValue() {
      return this.value;
    }

    @Override
    public int compareTo(EigenValue o) {
      // we want to sort eigenvalues in descending order
      return Double.compare(o.value, this.value);
    }
    
    @Override
    public String toString() {
      return "lamba: " + lambda + " value:" + value;
    }
  }
}
