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

public class DumbGroupGenerator {
	private static final int TOTAL_TICKS = 1000;
	private static final int VERTICES = 100;
	private static final int F_CENTER = 350;
	private static final int F_WIDTH = 1;
	private static final int MAX_X = 500;
	private static final int MAX_Y = 500;
	private static final int N_THREADS = 6;
	private static final int[] group = {0, 1, 2, 3};

	private static GroupVertex[] vertices = new GroupVertex[VERTICES];
	
	private static float movingP() {
		float lambda = 1.0f;
		Random rn = new Random();
		// random number with exponential distribution
		return (float) Math.log(1 - rn.nextFloat()) / (- lambda);
	}
	
	private static boolean isGroup(int id) {
		boolean ret = false;
		for (int i = 0; i < group.length; i++) {
			if (group[i] == id) {
				ret = true;
				break;
			}
		}
		return ret;
	}
	
	private static SpatialIndex init() {
		SpatialIndex index = new DumbIndex();
		Random rn = new Random();
		for (int i = 0; i < vertices.length; i++) {
			GroupVertex v = new GroupVertex(i, rn.nextInt(MAX_X), rn.nextInt(MAX_Y));
			vertices[i] = v;
			v.isGroup(isGroup(i));
			if (v.isGroup()) {
				v.x(F_CENTER);
				v.y(F_CENTER);
				//float f = v.x() / (float) MAX_X;
				v.color(Color.BLUE);
			} else {
				v.color(Color.RED);
			}
			index.add(v);
		}
		return index;
	}
	
	private static void updateTimeline(Map<GroupVertex,ArrayListMultimap<GroupVertex,Integer>> edgesTimeline, SortedSetMultimap<GroupVertex,GroupVertex> edges, ArrayListMultimap<GroupVertex,GroupVertex> verticesTimeline, GroupVertex[] vertices, int tick) {
		for (GroupVertex v: vertices) {
			// copy a snapshot of the current vertex state (position)
			verticesTimeline.put(v, new GroupVertex(v));
		}
		for (Entry<GroupVertex, Collection<GroupVertex>> v: edges.asMap().entrySet()) {
			GroupVertex key = v.getKey();
			ArrayListMultimap<GroupVertex,Integer> internal = edgesTimeline.get(key);
			if (internal == null) {
				internal = ArrayListMultimap.<GroupVertex,Integer>create(15, TOTAL_TICKS);
				edgesTimeline.put(key, internal);
			}
			for (GroupVertex dst: v.getValue()) {
				internal.put(dst, tick);
			}
		}
	}
	
	private static void spitTimeline(String filename, Map<GroupVertex,ArrayListMultimap<GroupVertex,Integer>> edgesTimeline, ArrayListMultimap<GroupVertex, GroupVertex> verticesTimeline) throws IOException {
		Writer out = new BufferedWriter(new FileWriter(new File(filename+".json")));
		out.write("{");
		for (Entry<GroupVertex, ArrayListMultimap<GroupVertex,Integer>> v: edgesTimeline.entrySet()) {
			GroupVertex src = v.getKey();
			out.write("\""+src.id()+"\": {");
			for (Entry<GroupVertex, Collection<Integer>> d: v.getValue().asMap().entrySet()) {
				GroupVertex dst = d.getKey();
				out.write("\""+dst.id()+"\": [");
				for (Integer i: d.getValue()) {
					out.write(i+",");
				}
				out.write("],");
			}
			out.write("}");
		}
		out.write("}");
		out.flush();
		out.close();
	}
	
	private static SortedSetMultimap<GroupVertex, GroupVertex> computeProximityGraph(SpatialIndex index) {
		System.out.println("computing proximtiy graph");
		ExecutorService executor = Executors.newFixedThreadPool(N_THREADS);
		SortedSetMultimap<GroupVertex,GroupVertex> edges = Multimaps.<GroupVertex,GroupVertex>synchronizedSortedSetMultimap(TreeMultimap.<GroupVertex,GroupVertex>create());
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
		SpatialIndex index = new DumbIndex();
		Random rn = new Random();
		for (int i = 0; i < vertices.length; i++) {
			GroupVertex v = vertices[i];
			int nx = 0, ny = 0;
			if (v.isGroup()) {
				switch (v.getState()) {
				case EAST: 
					if (v.x() > F_CENTER) { // turn 
						v.setState(GroupVertex.State.NORTH);
					} else { // continue
						nx = 20;
						ny = 0;
					}
					break;
				case NORTH: 
					if (v.y() > F_CENTER) { // turn 
						v.setState(GroupVertex.State.WEST);
					} else { // continue
						nx = 0;
						ny = 20;
					}
					break;
				case SOUTH: 
					if (v.y() < MAX_X - F_CENTER) { // turn 
						v.setState(GroupVertex.State.EAST);
					} else { // continue
						nx = 0;
						ny = -20;
					}
					break;
				case WEST: 
					if (v.x() < MAX_X - F_CENTER) { // turn 
						v.setState(GroupVertex.State.SOUTH);
					} else { // continue
						nx = -20;
						ny = 0;
					}
					break;
				}
			} else {
				nx = (int) (rn.nextGaussian() * 20);
				ny = (int) (rn.nextGaussian() * 20);
				// if it's moving out of screen, bounce
				if (v.x() + nx > MAX_X || v.x() + nx < 0) {
					nx *= - 1;
				}
				// if it's moving out of screen, bounce
				if (v.y() + ny > MAX_Y || v.y() + ny < 0) {
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
		Map<GroupVertex,ArrayListMultimap<GroupVertex,Integer>> edgesTimeline = new HashMap<GroupVertex, ArrayListMultimap<GroupVertex,Integer>>(VERTICES);
	    ArrayListMultimap<GroupVertex, GroupVertex> verticesTimeline = ArrayListMultimap.<GroupVertex,GroupVertex>create(VERTICES, TOTAL_TICKS); 
		OpenGLVisualizer vis = new OpenGLVisualizer(MAX_X, MAX_Y, 10);
		// scatter the vertices on the 2D plane and index their position
		SpatialIndex index = init();
		SortedSetMultimap<GroupVertex, GroupVertex> edges = computeProximityGraph(index);
		//Dumper.edgesDumper("mini_"+ 0, edges);
		updateTimeline(edgesTimeline, edges, verticesTimeline, vertices, 0);
		vis.draw(vertices, edges, false);
		for (int i = 1; i < TOTAL_TICKS; i++) {
			long start = System.currentTimeMillis();
			index = moveVertices();
			edges = computeProximityGraph(index);
			//Dumper.edgesDumper("mini_"+ i, edges);
			updateTimeline(edgesTimeline, edges, verticesTimeline, vertices, i);
			vis.draw(vertices, edges, false);
			//spitTimeline("sticazzi"+i, edgesTimeline, verticesTimeline);
			
			// print some stats
			System.out.println("Tick " + i + " computed in " + (System.currentTimeMillis() - start) + "ms.");
		}
		spitTimeline("group", edgesTimeline, verticesTimeline);
    }
}
