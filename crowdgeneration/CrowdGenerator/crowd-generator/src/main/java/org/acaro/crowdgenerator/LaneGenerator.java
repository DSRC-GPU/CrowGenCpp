package org.acaro.crowdgenerator;

import java.awt.Color;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Writer;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Queue;
import java.util.Random;
import java.util.SortedMap;
import java.util.SortedSet;
import java.util.concurrent.ConcurrentSkipListMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.acaro.crowdgenerator.indexing.DumbIndex;
import org.acaro.crowdgenerator.indexing.QuadTree;
import org.acaro.crowdgenerator.indexing.SpatialHash;
import org.acaro.crowdgenerator.indexing.SpatialIndex;
import org.acaro.crowdgenerator.visualizer.OpenGLVisualizer;
import org.apache.commons.math3.geometry.euclidean.oned.Interval;

import au.com.bytecode.opencsv.CSVWriter;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.Iterables;
import com.google.common.collect.LinkedHashMultimap;
import com.google.common.collect.Multimaps;
import com.google.common.collect.SortedSetMultimap;
import com.google.common.collect.TreeMultimap;

public class LaneGenerator {
  private static final int TOTAL_TICKS = 30;
  private static final int VERTICES = 10000;
  private static final int F_CENTER = 350;
  private static final int F_WIDTH = 50;
  private static final int MAX_X = 500;
  private static final int MAX_Y = 500;
  private static final int N_THREADS = 6;

  private static LaneVertex[] vertices = new LaneVertex[VERTICES];

  private static float movingP() {
    float lambda = 1.0f;
    Random rn = new Random();
    // random number with exponential distribution
    return (float) Math.log(1 - rn.nextFloat()) / (- lambda);
  }

  private static boolean isFlow(int x, int y) {
    return (y > F_CENTER - F_WIDTH && y < F_CENTER + F_WIDTH);
  }

  private static SpatialIndex init() {
    //SpatialIndex index = new SpatialHash(MAX_X, MAX_Y, 10);
    SpatialIndex index = new QuadTree();
    Random rn = new Random();
    for (int i = 0; i < vertices.length; i++) {
      LaneVertex v = new LaneVertex(i, rn.nextInt(MAX_X), rn.nextInt(MAX_Y));
      vertices[i] = v;
      v.isFlow(isFlow(v.x(), v.y()));
      if (v.isFlow()) {
        //float f = v.x() / (float) MAX_X;
        if (v.x() < 50) {
          v.color(Color.GREEN);
        } else {
          v.color(Color.BLUE);
        }
      } else {
        v.color(Color.RED);
      }
      index.add(v);
    }
    return index;
  }

  private static void updateTimeline(Map<LaneVertex,ArrayListMultimap<LaneVertex,Integer>> edgesTimeline, SortedSetMultimap<LaneVertex,LaneVertex> edges, ArrayListMultimap<LaneVertex,LaneVertex> verticesTimeline, LaneVertex[] vertices, int tick) {
    for (LaneVertex v: vertices) {
      // copy a snapshot of the current vertex state (position)
      verticesTimeline.put(v, new LaneVertex(v));
    }
    for (Entry<LaneVertex, Collection<LaneVertex>> v: edges.asMap().entrySet()) {
      LaneVertex key = v.getKey();
      ArrayListMultimap<LaneVertex,Integer> internal = edgesTimeline.get(key);
      if (internal == null) {
        internal = ArrayListMultimap.<LaneVertex,Integer>create(15, TOTAL_TICKS);
        edgesTimeline.put(key, internal);
      }
      for (LaneVertex dst: v.getValue()) {
        internal.put(dst, tick);
      }
    }
  }

  private static void spitTimeline(String filename, Map<LaneVertex,ArrayListMultimap<LaneVertex,Integer>> edgesTimeline, ArrayListMultimap<LaneVertex, LaneVertex> verticesTimeline) throws IOException {
    /*
     * Writing nodes file
     */
    Writer out = new BufferedWriter(new FileWriter(new File(filename+"_nodes.csv")));
    out.write("Id,Group,X,Y\n");
    CSVWriter csv = new CSVWriter(out, ',', CSVWriter.NO_QUOTE_CHARACTER, CSVWriter.NO_ESCAPE_CHARACTER);
    for (Entry<LaneVertex, Collection<LaneVertex>> v: verticesTimeline.asMap().entrySet()) {
      String x = Joiner.on(';').skipNulls().join((Iterables.transform(v.getValue(), new Function<LaneVertex,String>() {
        private int tick = 0;
        @Override
        public String apply(LaneVertex v) {
          return "[" + tick + "," + (tick++) + "," + v.x() + "]";
        }
      })).iterator());
      String y = Joiner.on(';').skipNulls().join((Iterables.transform(v.getValue(), new Function<LaneVertex,String>() {
        private int tick = 0;
        @Override
        public String apply(LaneVertex v) {
          return "[" + tick + "," + (tick++) + "," + v.y() + "]";
        }
      })).iterator());
      String[] items = new String[4];
      items[0] = String.valueOf(v.getKey().id());
      items[1] = String.valueOf(v.getKey().color().getRGB());
      //items[1] = String.valueOf(v.getKey().isFlow() ? 1 : 0);
      items[2] = "\"<" + x + ">\"";
      items[3] = "\"<" + y + ">\"";
      csv.writeNext(items);
    }
    csv.flush();
    csv.close();
    /*
     * Writing edges file
     */
    out = new BufferedWriter(new FileWriter(new File(filename+"_edges.csv")));
    out.write("Source,Target,Type,Id,Weight,Time Interval\n");
    csv = new CSVWriter(out, ',', CSVWriter.NO_QUOTE_CHARACTER, CSVWriter.NO_ESCAPE_CHARACTER);
    int eID = 0;
    for (Entry<LaneVertex, ArrayListMultimap<LaneVertex,Integer>> v: edgesTimeline.entrySet()) {
      LaneVertex src = v.getKey();
      for (Entry<LaneVertex, Collection<Integer>> d: v.getValue().asMap().entrySet()) {
        LaneVertex dst = d.getKey();
        Collection<Integer> ticks = d.getValue();
        Queue<Interval> intervals = new LinkedList<Interval>();
        int start = -2, last = -2;
        boolean open = true;
        for (Integer tick: ticks) {
          open = true;
          if (tick == last + 1) {
            last = tick;
          } else {
            if (start != -2) {
              intervals.add(new Interval(start, last));
              open = false;
            }
            start = last = tick;
          }
        }
        if (open) {
          intervals.add(new Interval(start, last));
        }
        String ints = Joiner.on(';').skipNulls().join((Iterables.transform(intervals, new Function<Interval,String>() {
          @Override
          public String apply(Interval i) {
            return "[" + (int) i.getLower() + "," + (int) i.getUpper() + "]";
          }
        })).iterator());
        String[] items = new String[6];
        items[0] = String.valueOf(src.id());
        items[1] = String.valueOf(dst.id());
        items[2] = "Undirected";
        items[3] = String.valueOf(eID++);
        items[4] = "1.0";
        items[5] = "\"<" + ints + ">\"";
        csv.writeNext(items);
      }
    }
    csv.flush();
    csv.close();
  }

  private static SortedSetMultimap<LaneVertex, LaneVertex> computeProximityGraph(SpatialIndex index) {
    System.out.println("computing proximtiy graph");
    ExecutorService executor = Executors.newFixedThreadPool(N_THREADS);
    SortedSetMultimap<LaneVertex,LaneVertex> edges = Multimaps.<LaneVertex,LaneVertex>synchronizedSortedSetMultimap(TreeMultimap.<LaneVertex,LaneVertex>create());
    for (int i = 0; i < N_THREADS; i++) {
      ProximityGraphGenerator g = new ProximityGraphGenerator(i, N_THREADS, vertices, edges, index);
      executor.execute(g);
    }
    executor.shutdown();
    while (!executor.isTerminated()) { }
    return edges;
  }

  private static SpatialIndex moveVertices() {
    System.out.println("moving vertices...");
    //SpatialIndex index = new SpatialHash(MAX_X, MAX_Y, 10);
    SpatialIndex index = new DumbIndex();
    Random rn = new Random();
    for (int i = 0; i < vertices.length; i++) {
      LaneVertex v = vertices[i];
      int nx, ny;
      if (v.isFlow()) {
        nx = (int) (movingP() * 20);
        ny = (int) (rn.nextGaussian() * 2);
        // if it's moving out of screen, bounce
        if (v.x() + nx > MAX_X) {
          nx = - v.x() + ((v.x() + nx) - MAX_X);
        }
        // if it's moving out of flow, bounce
        if (!isFlow(v.x() + nx, v.y() + ny)) {
          ny *= - 1;
        }
      } else {
        nx = (int) (rn.nextGaussian() * 2);
        ny = (int) (rn.nextGaussian() * 2);
        // if it's moving out of screen, bounce
        if (v.x() + nx > MAX_X || v.x() + nx < 0) {
          nx *= - 1;
        }
        // if it's moving out of screen or inside flow, bounce
        if (v.y() + ny > MAX_Y || v.y() + ny < 0 || isFlow(v.x() + nx, v.y() + ny)) {
          ny *= - 1;
        }
      }
      v.x(v.x() + nx);
      v.y(v.y() + ny);
      index.add(v);
    }
    return index;
  }

  public static void main(String[] args) throws InterruptedException, IOException {
    Map<LaneVertex,ArrayListMultimap<LaneVertex,Integer>> edgesTimeline = new HashMap<LaneVertex, ArrayListMultimap<LaneVertex,Integer>>(VERTICES);
    ArrayListMultimap<LaneVertex, LaneVertex> verticesTimeline = ArrayListMultimap.<LaneVertex,LaneVertex>create(VERTICES, TOTAL_TICKS); 
    OpenGLVisualizer vis = new OpenGLVisualizer(MAX_X, MAX_Y, 10);
    // scatter the vertices on the 2D plane and index their position
    SpatialIndex index = init();
    // compute the first initial proximity graph
    SortedSetMultimap<LaneVertex, LaneVertex> edges = computeProximityGraph(index);
    Dumper.edgesDumper("mini_"+ 0, edges);
    updateTimeline(edgesTimeline, edges, verticesTimeline, vertices, 0);
    vis.draw(vertices, edges, false);
    for (int i = 1; i < TOTAL_TICKS; i++) {
      long start = System.currentTimeMillis();
      index = moveVertices();
      edges = computeProximityGraph(index);
      Dumper.edgesDumper("mini_"+ i, edges);
      updateTimeline(edgesTimeline, edges, verticesTimeline, vertices, i);
      vis.draw(vertices, edges, false);
      //spitTimeline("sticazzi"+i, edgesTimeline, verticesTimeline);

      // print some stats
      System.out.println("Tick " + i + " computed in " + (System.currentTimeMillis() - start) + "ms.");
    }
    spitTimeline("mini", edgesTimeline, verticesTimeline);
  }
}
