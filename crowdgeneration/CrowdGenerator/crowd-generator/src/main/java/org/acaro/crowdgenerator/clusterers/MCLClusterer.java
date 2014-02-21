package org.acaro.crowdgenerator.clusterers;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.StringTokenizer;

import org.acaro.crowdgenerator.Vertex;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;

public class MCLClusterer {
  
  public static Map<Integer,Collection<Integer>> cluster(Map<Vertex,Map<Vertex,Integer>> aggregateGraph, int total, int vertices) throws IOException {
    Map<Integer,Collection<Integer>> clusters = Maps.newHashMap();
    
    ProcessBuilder pb = new ProcessBuilder(
        "/usr/local/bin/mcl", 
        "-", // read from stdin 
        "--abc", // read edge-based format
        "-I", "1.1",  // inflate
        "-pi", "20.0", // pre-inflate
        "-te", "8",  // use 8 threads for expansion
        "-o", "-" // write to stdout
        );
    
    Process p = pb.start();
    
    BufferedWriter out = new BufferedWriter(new OutputStreamWriter(p.getOutputStream()));
    BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));

    for (Entry<Vertex, Map<Vertex, Integer>> entry : aggregateGraph.entrySet()) {
      for (Entry<Vertex, Integer> edge : entry.getValue().entrySet()) {
        float weight = (float) edge.getValue() / total;
        //System.out.println("Spitting edge " + entry.getKey().id() + " " + edge.getKey().id() + " " + weight);
        out.write(entry.getKey().id() + " " + edge.getKey().id() + " " + weight + "\n");
      }
    }
    out.close();

    int clusterId = 0;
    String line;
    while ((line = in.readLine()) != null) {
      List<Integer> cluster = Lists.newArrayList();
      StringTokenizer st = new StringTokenizer(line, " \t");
      while (st.hasMoreTokens()) {
        cluster.add(Integer.valueOf(st.nextToken()));
      }
      clusters.put(clusterId++, cluster);
    }
    
    return clusters;
  }
}
