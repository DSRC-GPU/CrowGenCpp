package org.acaro.crowdgenerator;

import java.util.SortedMap;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;

import org.acaro.crowdgenerator.visualizer.OpenGLVisualizer;

public class Grid {

	public static void main(String[] args) {
		LaneVertex grid[][] = new LaneVertex[50][50];
		LaneVertex vertices[] = new LaneVertex[2500];
		for (int i = 0, k = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				LaneVertex v = new LaneVertex(k, i*10, j*10);
				grid[i][j] = v;
				vertices[k++] = v;
			}
		}
		
		SortedMap<LaneVertex, SortedSet<LaneVertex>> edges = new TreeMap<LaneVertex, SortedSet<LaneVertex>>();
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				LaneVertex v = grid[i][j];
				SortedSet<LaneVertex> internal = new TreeSet<LaneVertex>();
				edges.put(v, internal);
				if (i > 0) {
					internal.add(grid[i-1][j]);
				}
				if (j > 0) {
					internal.add(grid[i][j-1]);
				}
			}
		}
		
		OpenGLVisualizer vis = new OpenGLVisualizer(500, 500, 10);
		vis.draw(vertices, edges);
//		VerticesVisualizer vis = new VerticesVisualizer(500, 500);
//		vis.draw(vertices);
	}
}
