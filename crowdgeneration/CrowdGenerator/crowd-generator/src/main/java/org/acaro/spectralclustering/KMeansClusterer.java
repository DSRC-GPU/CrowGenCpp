package org.acaro.spectralclustering;

import java.util.List;
import java.util.Random;
import java.util.Set;

import org.ejml.simple.SimpleMatrix;

import com.google.common.collect.Lists;
import com.google.common.collect.Sets;

public class KMeansClusterer {
  private SimpleMatrix data;
  
  public KMeansClusterer(SimpleMatrix data) {
    this.data = data;
  }
  
  private double computeDistance(SimpleMatrix v1, SimpleMatrix v2) {
    SimpleMatrix distanceMatrix;
    distanceMatrix = v1.minus(v2);
    distanceMatrix = distanceMatrix.elementMult(distanceMatrix);
    return distanceMatrix.elementSum() / v1.numCols();
  }
  
  public List<Centroid> chooseRandomCentroids(int k) {
    List<Centroid> centroids = Lists.newArrayListWithCapacity(k);
    Set<Integer> chosenCentroids = Sets.newHashSetWithExpectedSize(k);
    Random rnd = new Random();

    while (centroids.size() != k) {
      int p;
      do {
        p = rnd.nextInt(data.numRows());
      } while (chosenCentroids.contains(p));
      SimpleMatrix centroid = new SimpleMatrix(1, data.numCols());
      for (int j = 0; j < data.numCols(); j++) {
        centroid.set(0, j, data.get(p, j));
      }
      chosenCentroids.add(p);
      centroids.add(new Centroid(centroid));
    }
    return centroids;
  }
  
  public List<Centroid> chooseCentroids(int ... userCentroids) {
    List<Centroid> centroids = Lists.newArrayListWithCapacity(userCentroids.length);

    for (int c : userCentroids) {
      SimpleMatrix centroid = new SimpleMatrix(1, data.numCols());
      for (int j = 0; j < data.numCols(); j++) {
        centroid.set(0, j, data.get(c, j));
      }
      centroids.add(new Centroid(centroid));
    }
    return centroids;
  }
  
  public List<Centroid> cluster(List<Centroid> centroids) {
    int iteration = 0;
    boolean hasConverged = false;
    while (!hasConverged) {
      double startTime = System.currentTimeMillis();
      // Step 1: assign each point to a centroid
      for (Centroid centroid : centroids) {
        centroid.getDataPoints().clear();
      }
      // iterate on each point and choose the closest centroid
      for (int i = 0; i < data.numRows(); i++) {
        Centroid selectedCentroid = null;
        double minDistanceWithCentroid = Double.POSITIVE_INFINITY;
        SimpleMatrix dataVector = data.extractVector(true, i);
        for (Centroid centroid : centroids) {
          double distanceWithCentroid = computeDistance(dataVector, centroid.getFeatures());
          if (distanceWithCentroid < minDistanceWithCentroid) {
            selectedCentroid = centroid;
            minDistanceWithCentroid = distanceWithCentroid;
          }
        }
        selectedCentroid.getDataPoints().add(i);
      }
      
      // Step 2: move the centroids to the average of the assigned points
      hasConverged = true;
      for (Centroid centroid : centroids) {
        SimpleMatrix averageVector = new SimpleMatrix(1, data.numCols());
        for (Integer point : centroid.getDataPoints()) {
          averageVector = averageVector.plus(data.extractVector(true, point));
        }
        averageVector = averageVector.divide(centroid.getDataPoints().size());
        if (!centroid.getFeatures().isIdentical(averageVector, 0)) {
          hasConverged = false;
          centroid.setFeatures(averageVector);
        }
      }
      //System.out.println(
      //    "Iteration " + iteration++ + " computed in " + (System.currentTimeMillis() - startTime) + " ms.");
    }
    return centroids;
  }
  
  public double computeError(List<Centroid> centroids) {
    double error = 0;
    for (Centroid centroid : centroids) {
      for (Integer p : centroid.getDataPoints()) {
        error += computeDistance(data.extractVector(true, p), centroid.getFeatures());
      }
    }
    return error / data.numRows();
  }
  
/*  public double computeError(List<Centroid> centroids) {
    double[][] errorMatrix = new double[data.numRows()][data.numCols()]; 
    for (Centroid centroid : centroids) {
      double[] features = new double[data.numCols()];
      for (int j = 0; j < centroid.getFeatures().numCols(); j++) {
        features[j] = centroid.getFeatures().get(0, j);
      }
      for (Integer p : centroid.getDataPoints()) {
        errorMatrix[p] = features; 
      }
    }
    
    SimpleMatrix errorSimpleMatrix = new SimpleMatrix(errorMatrix).minus(data);
    System.out.println(errorSimpleMatrix);
    return errorSimpleMatrix.mult(errorSimpleMatrix.transpose()).elementSum() / (data.numCols() * data.numRows());
  }*/

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
    
    KMeansClusterer clusterer = new KMeansClusterer(new SimpleMatrix(data));
    List<Centroid> centroids = clusterer.chooseCentroids(2, 13);
    //List<Centroid> centroids = clusterer.chooseRandomCentroids(2);
    List<Centroid> results = clusterer.cluster(centroids);
    double error = clusterer.computeError(results);
    
    System.out.println("KMeans computed for " + centroids.size() + " clusters with error " + error);
    for (Centroid centroid : results) {
      System.out.println("Centroid " + centroid.getFeatures().toString());
    }
  }
  
  public class Centroid {
    private List<Integer> points;
    private SimpleMatrix features;
    
    public Centroid(SimpleMatrix features) {
      this.features = features;
      this.points = Lists.newLinkedList();
    }
    
    public List<Integer> getDataPoints() {
      return points;
    }
    
    public void setFeatures(SimpleMatrix features) {
      this.features = features;
    }
    
    public SimpleMatrix getFeatures() {
      return features;
    }
    
    @Override
    public String toString() {
      return features.toString();
    }
  }
}
