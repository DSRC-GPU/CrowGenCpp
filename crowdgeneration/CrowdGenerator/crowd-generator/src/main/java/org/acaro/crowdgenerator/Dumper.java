package org.acaro.crowdgenerator;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Map.Entry;
import java.util.SortedMap;
import java.util.SortedSet;

import com.google.common.collect.SortedSetMultimap;

public class Dumper {
	
	public static void verticesDumper(String filename, LaneVertex[] vertices) throws IOException {
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(new File(filename+"_nodes.csv"))), true);
		out.println("Id,Flow,X,Y");
		for (int i = 0; i < vertices.length; i++) {
			out.println((float)vertices[i].id() + "," + (vertices[i].isFlow()? 1 : 0) + "," + vertices[i].x() + "," + vertices[i].y());
		}
		out.flush();
		out.close();
	}
	
	public static void edgesDumper(String filename, SortedSetMultimap<LaneVertex, LaneVertex> edges) throws IOException {
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(new File(filename+"_edges.csv"))), true);
		out.println("Source,Target,Type,Id,Weight");
		int i = 0;
		for (Entry<LaneVertex, LaneVertex> e: edges.entries()) {
			out.println(e.getKey().id() + "," + e.getValue().id() + ",Undirected," + i++ + ",1.0");
		}
		out.flush();
		out.close();
	}
	
	public static void graphDumper(String filename, SortedSetMultimap<LaneVertex, LaneVertex> edges, LaneVertex[] vertices) throws IOException {
		edgesDumper(filename, edges);
		verticesDumper(filename, vertices);
	}
}
