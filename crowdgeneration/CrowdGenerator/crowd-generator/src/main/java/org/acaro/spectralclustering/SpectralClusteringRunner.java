package org.acaro.spectralclustering;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.acaro.spectralclustering.KMeansClusterer.Centroid;

import com.google.common.collect.Lists;

public class SpectralClusteringRunner {
    public static final int MAX_X = 800;
    public static final int MAX_Y = 800;
    public static final int X = 0;
    public static final int Y = 1;
  
    public static double translate(double value, double aMax, double aMin, double bMax, double bMin) {
      double aSpan = aMax - aMin;
      double bSpan = bMax - bMin;
      
      double valueScaled = (value - aMin) / aSpan;
      double valueTranslated = bMin + (valueScaled * bSpan);
      
      return valueTranslated;
    }
    
    public static void main(String[] args) throws IOException {
      List<double[]> fileContent = Lists.newLinkedList();
      double maxX = Double.NEGATIVE_INFINITY, minX = Double.POSITIVE_INFINITY, maxY = Double.NEGATIVE_INFINITY, minY = Double.POSITIVE_INFINITY;
      BufferedReader br = new BufferedReader(new FileReader(args[0]));
      String line;
      while ((line = br.readLine()) != null) {
        String[] tokens = line.trim().split("[ \t]");
        double[] d = new double[2];
        d[X] = Double.parseDouble(tokens[X]);
        d[Y] = Double.parseDouble(tokens[Y]);
        maxX = Math.max(maxX, d[X]);
        maxY = Math.max(maxY, d[Y]);
        minX = Math.min(minX, d[X]);
        minY = Math.min(minY, d[Y]);
        fileContent.add(d);
      }
      br.close();

      double[][] data = new double[fileContent.size()][2];      
      int i = 0;
      for (double[] d : fileContent) {
        d[X] = translate(d[X], maxX, minX, MAX_X, 0);
        d[Y] = translate(d[Y], maxY, minY, MAX_Y, 0);
        //System.out.println("Adding " + Arrays.toString(d));
        data[i++] = d;
      }
      
      SpectralClustering clusterer = new SpectralClustering(data, 2);
      List<Centroid> clusters = clusterer.cluster();
      
      OpenGLVisualizer vis = new OpenGLVisualizer(MAX_X, MAX_Y);
      vis.draw(data, clusters);
    }
}
