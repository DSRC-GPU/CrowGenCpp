package org.acaro.crowdgenerator.indexing;

import org.acaro.crowdgenerator.Vertex;

public interface SpatialIndex {
	public Vertex add(Vertex v);
	public Iterable<Vertex> getVertices(int x, int y, float radius);
}
