package org.acaro.crowdgenerator;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

public class Timeline {
	private SortedMap<Integer, Queue<LaneVertex>> vertices;
	private SortedMap<Integer, Map<LaneVertex, Set<LaneVertex>>> edges;
	private int start, end;
	
	public Timeline(int start, int end) {
		this.start = start;
		this.end   = end;
		vertices = new TreeMap<Integer, Queue<LaneVertex>>();
		edges    = new TreeMap<Integer, Map<LaneVertex, Set<LaneVertex>>>();
		for (int i = start; i < end; i++) {
			vertices.put(i, new LinkedList<LaneVertex>());
			edges.put(i, new ConcurrentHashMap<LaneVertex, Set<LaneVertex>>());
		}
	}
	
	public Queue<LaneVertex> getVertices(int tick) {
		if (tick >= end || tick < start) {
			throw new RuntimeException("tick out of range: " + tick);
		}
		return vertices.get(tick);
	}
	
	public Map<LaneVertex, Set<LaneVertex>> getEdges(int tick) {
		if (tick >= end || tick < start) {
			throw new RuntimeException("tick out of range: " + tick);
		}
		return edges.get(tick);
	}
}
