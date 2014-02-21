package org.acaro.crowdgenerator.indexing;

import java.util.Iterator;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.acaro.crowdgenerator.Vertex;

public class DumbIndex implements SpatialIndex {
	private Queue<Vertex> vertices;

	public DumbIndex() {
		this.vertices = new ConcurrentLinkedQueue<Vertex>();
	}
	
	@Override
	public Vertex add(Vertex v) {
		this.vertices.add(v);
		return v;
	}

	@Override
	public Iterable<Vertex> getVertices(int x, int y, float r) {
		return new RadiusIterable(x, y, r, this.vertices);
	}
	
	private class RadiusIterable implements Iterable<Vertex>, Iterator<Vertex> {
		private Iterator<Vertex> vertices;
		private Vertex next;
		private int x, y;
		private float r;
		
		public RadiusIterable(int x, int y, float r, Iterable<Vertex> vertices) {
			this.vertices = vertices.iterator();
			this.x = x;
			this.y = y;
			this.r = r;
			this.next = advance();
		}
		
        private float distance(int x1, int y1, int x2, int y2) {
            return (float) Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
        }
        
		private Vertex advance() {
			while (vertices.hasNext()) {
			  Vertex tmp = vertices.next();
				if (distance(x, y, tmp.x(), tmp.y()) <= r) {
					return tmp;
				}
			}
			return null;
		}
		
		@Override
		public boolean hasNext() {
			return next != null;
		}

		@Override
		public Vertex next() {
		  Vertex last = next;
			next = advance();
			
			return last;
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException("can't modify iterator");			
		}

		@Override
		public Iterator<Vertex> iterator() {
			return this;
		}
	}
}
