package org.acaro.crowdgenerator;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Collection;
import java.util.Map;

import org.acaro.crowdgenerator.clusterers.RMCLClusterer;

import com.google.common.collect.Maps;

public class PlaygroundClusterer {

  public static void main(String[] args) throws IOException {
    Map<Vertex,Map<Vertex,Integer>> aggregateGraph = Maps.newHashMap();
    BufferedReader in = new BufferedReader(new FileReader(args[0]));
    int maxvertices = -1;
    
    String line;
    while ((line = in.readLine()) != null) {
      String[] tokens = line.split("[ \t]");
      if (tokens.length != 2) {
        continue;
      }
      Vertex id1 = new Vertex(Integer.parseInt(tokens[0]));
      Vertex id2 = new Vertex(Integer.parseInt(tokens[1]));
      Vertex src, dst;
      maxvertices = Math.max(id1.id(), maxvertices);
      maxvertices = Math.max(id2.id(), maxvertices);
      if (id1.id() < id2.id()) {
        src = id1;
        dst = id2;
      } else {
        dst = id1;
        src = id2;
      }
      Map<Vertex,Integer> internal = aggregateGraph.get(src);
      if (internal == null) {
        internal = Maps.newHashMap();
        aggregateGraph.put(src, internal);
      }
      internal.put(dst, 1);
    }
    in.close();
    System.out.println("graph loaded: " + aggregateGraph.size());
    Map<Integer,Collection<Integer>> clusters =
        RMCLClusterer.cluster(aggregateGraph, 1, maxvertices+1);
    for (Collection<Integer> cluster : clusters.values()) {
      System.out.println(cluster);
    }
  }
}
