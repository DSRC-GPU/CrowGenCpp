package org.acaro.crowdgenerator.clusterers;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.Map.Entry;

import org.acaro.crowdgenerator.Vertex;

import RRW.RRW;

import com.google.common.collect.Lists;
import com.google.common.collect.Sets;

public class RRWClusterer {
  public static List<List<Integer>> cluster(Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total) throws IOException {
    List<List<Integer>> clusters = Lists.newArrayList();
    
    File outputfile = new File("tmpoutputfile.txt");
    if (outputfile.exists()) {
      outputfile.delete();
    }
    outputfile.createNewFile();

    File resultfile = new File("resultfile.txt");
    if (resultfile.exists()) {
      resultfile.delete();
    }
    
    BufferedWriter out = new BufferedWriter(new FileWriter(outputfile.getAbsoluteFile()));
    for (Entry<Vertex, Map<Vertex, Integer>> entry : aggregateGraph.entrySet()) {
      for (Entry<Vertex, Integer> edge : entry.getValue().entrySet()) {
        float weight = (float) edge.getValue() / total;
        //System.out.println("Spitting edge " + entry.getKey().id() + " " + edge.getKey().id() + " " + weight);
        out.write(entry.getKey().id() + "\t" + edge.getKey().id() + "\t" + weight + "\n");
      }
    }
    out.close();
    
    String[] parameters = new String[] {
      "-i",
      outputfile.getAbsolutePath(),
      "-o",
      resultfile.getAbsolutePath(),
      "-max",
      "10000",
      "-min",
      "1",
      "-overlap",
      "0.5"
    };
    
    RRW rrw = new RRW(parameters);
    
    BufferedReader in = new BufferedReader(new FileReader(resultfile.getAbsoluteFile()));
    Set<Integer> assigned = Sets.newHashSet();
    String line;
    while ((line = in.readLine()) != null) {
      List<Integer> cluster = Lists.newArrayList();
      StringTokenizer st = new StringTokenizer(line, " \t");
      while (st.hasMoreTokens()) {
        Integer id = Integer.valueOf(st.nextToken());
        if (!assigned.contains(id)) {
          cluster.add(id);
          assigned.add(id);
        }
      }
      clusters.add(cluster);
    }
    in.close();
    
    return clusters;
  }
}
