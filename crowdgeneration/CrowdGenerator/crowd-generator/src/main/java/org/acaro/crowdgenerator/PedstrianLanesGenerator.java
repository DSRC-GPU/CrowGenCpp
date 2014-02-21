package org.acaro.crowdgenerator;

import it.uniroma1.dis.wsngroup.gexf4j.core.EdgeType;
import it.uniroma1.dis.wsngroup.gexf4j.core.Gexf;
import it.uniroma1.dis.wsngroup.gexf4j.core.Graph;
import it.uniroma1.dis.wsngroup.gexf4j.core.Mode;
import it.uniroma1.dis.wsngroup.gexf4j.core.Node;
import it.uniroma1.dis.wsngroup.gexf4j.core.dynamic.Spell;
import it.uniroma1.dis.wsngroup.gexf4j.core.dynamic.TimeFormat;
import it.uniroma1.dis.wsngroup.gexf4j.core.impl.GexfImpl;
import it.uniroma1.dis.wsngroup.gexf4j.core.impl.SpellImpl;
import it.uniroma1.dis.wsngroup.gexf4j.core.impl.StaxGraphWriter;

import java.awt.Color;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.lang.reflect.Array;
import java.util.Calendar;
import java.util.Collection;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.acaro.crowdgenerator.clusterers.NewSpectralClusterer;
import org.acaro.crowdgenerator.clusterers.SpectralClusterer;
import org.acaro.crowdgenerator.indexing.QuadTree;
import org.acaro.crowdgenerator.indexing.SpatialIndex;
import org.acaro.crowdgenerator.visualizer.CrowdiologistFrame;
import org.acaro.crowdgenerator.visualizer.EigenSpace;
import org.apache.commons.math3.geometry.euclidean.oned.Interval;

import au.com.bytecode.opencsv.CSVWriter;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.Iterables;
import com.google.common.collect.Maps;
import com.google.common.collect.Multimaps;
import com.google.common.collect.SortedSetMultimap;
import com.google.common.collect.TreeMultimap;

public class PedstrianLanesGenerator {
  private static final int TOTAL_TICKS = 200;
  private static final int VERTICES = 60;
  private static final int MAX_X = 1200;
  private static final int MAX_Y = 220;
  private static final int N_THREADS = 6;
  private static final int WINDOW_SIZE = 40;
  private static final double EXP = 2.0;

  private static Vertex[] vertices = new LaneVertex[VERTICES];

  private static float movingP() {
    float lambda = 1.0f;
    Random rn = new Random();
    // random number with exponential distribution
    return (float) Math.log(1 - rn.nextFloat()) / (- lambda);
  }

  private static SpatialIndex init() {
    int BORDER = 30;
    SpatialIndex index = new QuadTree();
    int id = 0;
    for (int x = BORDER, i = 0; i < vertices.length / 6; x += BORDER, i++) {
      for (int y = BORDER, j = 0; j < 3; y += BORDER, j++) {
        System.out.println("Creating vertex " + id);
        Vertex v = new LaneVertex(id, x, y);
        vertices[id] = v;
        index.add(v);
        id++;
      }
    }
    for (int x = MAX_X - BORDER, i = 0; i < vertices.length / 6; x -= BORDER, i++) {
      for (int y = MAX_Y - BORDER, j = 0; j < 3; y -= BORDER, j++) {
        System.out.println("Creating vertex " + id);
        Vertex v = new LaneVertex(id, x, y);
        vertices[id] = v;
        index.add(v);
        id++;
      }
    }
    return index;
  }

  private static void spitTimeline(String filename, SortedSetMultimap<Vertex,Edge>[] proximityGraphs) { 
    Gexf gexf = new GexfImpl();
    Calendar date = Calendar.getInstance();
    
    gexf.getMetadata()
      .setLastModified(date.getTime())
      .setCreator("crowd-generator")
      .setDescription("Crowd Corridor Two Lanes");
    
    Graph graph = gexf.getGraph();
    graph.setDefaultEdgeType(EdgeType.UNDIRECTED).setMode(Mode.DYNAMIC).setTimeType(TimeFormat.INTEGER);
    Map<Integer,Node> nodes = Maps.newHashMapWithExpectedSize(VERTICES);    
    
    /*
     * Create vertices 
     */ 
    for (Vertex v : vertices) {
      Node n = graph.createNode(String.valueOf(v.id()));
      n.setLabel(String.valueOf(v.id()));
      Spell s = new SpellImpl();
      s.setStartValue(0);
      s.setEndValue(TOTAL_TICKS);
      n.getSpells().add(s);
      nodes.put(v.id(), n);
    }
    
    Map<Vertex,ArrayListMultimap<Vertex,Integer>> aggregateGraph = Maps.newHashMap();
    for (int i = 0; i < TOTAL_TICKS; i++) {
      SortedSetMultimap<Vertex,Edge> proximityGraph = proximityGraphs[i];
      for (Entry<Vertex,Edge> edge : proximityGraph.entries()) {
        ArrayListMultimap<Vertex,Integer> internal = aggregateGraph.get(edge.getKey());
        if (internal == null) {
          internal = ArrayListMultimap.create();
          aggregateGraph.put(edge.getKey(), internal);
        }
        internal.put(edge.getValue().getDst(), i);
      }
    }
    
    for (Entry<Vertex,ArrayListMultimap<Vertex,Integer>> v : aggregateGraph.entrySet()) {
      Vertex src = v.getKey();
      Node srcNode = nodes.get(src.id());
      for (Entry<Vertex,Collection<Integer>> e : v.getValue().asMap().entrySet()) {
        Vertex dst = e.getKey();
        Node dstNode = nodes.get(dst.id());
        it.uniroma1.dis.wsngroup.gexf4j.core.Edge edge = srcNode.connectTo(dstNode);
        int start = -2, last = -2;
        boolean open = true;
        for (Integer tick : e.getValue()) {
          open = true;
          if (tick == last + 1) {
            last = tick;
          } else {
            if (start != -2) {
              Spell s = new SpellImpl();
              s.setStartValue(start);
              s.setEndValue(last);
              edge.getSpells().add(s);
              open = false;
            }
            start = last = tick;
          }
        }
        if (open) {
          Spell s = new SpellImpl();
          s.setStartValue(start);
          s.setEndValue(last);
          edge.getSpells().add(s);
        }
      }
    }
    
    StaxGraphWriter graphWriter = new StaxGraphWriter();
    File f = new File(filename);
    Writer out;
    try {
      out =  new FileWriter(f, false);
      graphWriter.writeToStream(gexf, out, "UTF-8");
      System.out.println(f.getAbsolutePath());
    } catch (IOException e) {
      e.printStackTrace();
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

  private static SortedSetMultimap<Vertex,Edge> computeProximityGraph(SpatialIndex index) {
    System.out.println("computing proximity graph");
    ExecutorService executor = Executors.newFixedThreadPool(N_THREADS);
    SortedSetMultimap<Vertex,Edge> edges = Multimaps.<Vertex,Edge>synchronizedSortedSetMultimap(TreeMultimap.<Vertex,Edge>create());
    for (int i = 0; i < N_THREADS; i++) {
      ProximityGraphGenerator g = new ProximityGraphGenerator(i, N_THREADS, vertices, edges, index);
      executor.execute(g);
    }
    executor.shutdown();
    while (!executor.isTerminated()) { }
    return edges;
  }
  
  private static SortedSetMultimap<Vertex,Edge> aggregateProximityGraphs(SortedSetMultimap<Vertex,Edge>[] proximityGraphs, int start, int end) {
    Map<Vertex,Map<Vertex,Integer>> aggregateGraph = Maps.newHashMap();
    
    for (int i = start; i < end; i++) {
      SortedSetMultimap<Vertex,Edge> graph = proximityGraphs[i];
      for (Entry<Vertex,Edge> entry : graph.entries()) {
        Vertex src = entry.getKey();
        Edge edge = entry.getValue();
        Vertex dst = edge.getDst();
        Map<Vertex,Integer> internal = aggregateGraph.get(src);
        if (internal == null) {
          internal = Maps.newHashMap();
          aggregateGraph.put(src, internal);
        }
        Integer count = internal.get(dst);
        if (count == null) {
          count = 0;
        }
        internal.put(dst, count + 1);
      }
    }
    
    int length = end - start;
    SortedSetMultimap<Vertex,Edge> weightedEdges = TreeMultimap.<Vertex,Edge>create();
    for (Entry<Vertex, Map<Vertex,Integer>> internal : aggregateGraph.entrySet()) {
      Vertex src = internal.getKey();
      for (Entry<Vertex,Integer> entry : internal.getValue().entrySet()) {
        Vertex dst = entry.getKey();
        double weight = entry.getValue() / (double) length;
        weightedEdges.put(src, new Edge(dst, Math.pow(weight, EXP)));
      }
    }
    
    return weightedEdges;
  }

  private static SpatialIndex moveVertices() {
    System.out.println("moving vertices...");
    SpatialIndex index = new QuadTree();
    Random rnd = new Random();
    for (int i = 0; i < vertices.length; i++) {
      Vertex v = vertices[i];
      int x = v.x();
      int y = v.y();
      if (v.id() < vertices.length / 2) {
        // uncomment these (and comment next) to have less grid-y lane
        //v.x(x + rnd.nextInt(5));
        //v.y(y + - 1 + rnd.nextInt(3));
        v.x(x + 5);
      } else {
        // uncomment these (and comment next) to have less grid-y lane
        //v.x(x - rnd.nextInt(5));
        //v.y(y + - 1 + rnd.nextInt(3));
        v.x(x - 5);
      }
      index.add(v);
    }
    return index;
  }
  
  public static void main(String[] args) throws InterruptedException, IOException {
    @SuppressWarnings("unchecked")
    SortedSetMultimap<Vertex,Edge>[] proximityGraphs =
      (SortedSetMultimap<Vertex,Edge>[]) Array.newInstance(SortedSetMultimap.class, TOTAL_TICKS);
    double[] eigenvalues = new double[VERTICES];
    CrowdiologistFrame frame = new CrowdiologistFrame(MAX_X, MAX_Y);
    EigenSpace eispace = new EigenSpace(300, 300);
    // scatter the vertices on the 2D plane and index their position
    SpatialIndex index = init();
    // compute the first initial proximity graph
    SortedSetMultimap<Vertex,Edge> edges = computeProximityGraph(index);
    proximityGraphs[0] = edges;
    frame.getSimulatorPanel().draw(vertices, edges);
    for (int i = 1; i < TOTAL_TICKS; i++) {
      long startTime = System.currentTimeMillis();
      index = moveVertices();
      edges = computeProximityGraph(index);
      proximityGraphs[i] = edges;
      if (i >= WINDOW_SIZE) {
        edges = aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i);
        Map<Integer,Collection<Integer>> clusters =
            //MCLClusterer.cluster(aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i), WINDOW_SIZE, VERTICES);
            //RMCLClusterer.cluster(aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i), WINDOW_SIZE, VERTICES);
            //SpatialClusterer.cluster(aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i), WINDOW_SIZE, VERTICES);
            //MySpatialClusterer.cluster(aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i), WINDOW_SIZE, VERTICES);
            //AndreasClusterer.cluster(aggregateProximityGraphs(proximityGraphs, i - WINDOW_SIZE, i), WINDOW_SIZE, VERTICES);
            //SpectralClusterer.cluster(edges, VERTICES, eigenvalues);
            NewSpectralClusterer.cluster(edges, VERTICES, eigenvalues, eispace);
        System.out.println("Found " + clusters.size() + " clusters");
        for (Vertex v : vertices) {
          v.color(Color.white);
        }
        int clusterId = 0;
        for (Collection<Integer> cluster : clusters.values()) {
          Color color = PedestrianLaneVertex.convertToColor(clusterId);
          for (Integer id : cluster) {
            vertices[id].color(color);
          }
          clusterId++;
        }
      }
      frame.getSimulatorPanel().draw(vertices, edges);
      frame.getHistogramPanel().draw(eigenvalues, true);
      // print some stats
      long computedTime = System.currentTimeMillis() - startTime;
      if (computedTime < 120) {
        Thread.sleep(120 - computedTime);
      }
      System.out.println("Tick " + i + " computed in " + computedTime + "ms.");
    }
    spitTimeline("lanes.gexf", proximityGraphs);
  }
}
