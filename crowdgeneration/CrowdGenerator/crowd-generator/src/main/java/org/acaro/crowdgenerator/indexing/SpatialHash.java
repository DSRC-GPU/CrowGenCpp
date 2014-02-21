package org.acaro.crowdgenerator.indexing;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.acaro.crowdgenerator.Vertex;


public class SpatialHash implements SpatialIndex {
	private final int width, heigth;
	private final float f; // factor, used to avoid divisions
	private final int w; // number of cells on the X axis
	/*
	 * In principle this could be a HashMap. We would win space (empty cells would be
	 * not stored), we would loose performance (another level of hashing and access).
	 */
	private ArrayList<Queue<Vertex>> cells;
	
	public SpatialHash(int width, int heigth, int cellSize) {
		this.width = width;
		this.heigth = heigth;
		this.f = 1f / cellSize;
		this.w  = width / cellSize;
		if (width % cellSize != 0 || heigth % cellSize != 0) {
			throw new RuntimeException("width and height should be proportional to cell size");
		}
		int numOfRows = width / cellSize;
		int numOfCols = heigth / cellSize;
		cells = new ArrayList<Queue<Vertex>>(numOfCols * numOfRows);
		for (int i = 0; i < numOfCols * numOfRows; i++) {
			cells.add(i, new ConcurrentLinkedQueue<Vertex>());
		}
	}
	
	@Override
	public Vertex add(Vertex v) { 
		int cellID = index(v.x(), v.y());
		if (cellID < 0 || cellID >= cells.size()) {
            throw new RuntimeException("Illegal cellID " + cellID);
        }
        cells.get(cellID).add(v);

		return v; 
	}
	
	@Override
	public Iterable<Vertex> getVertices(int x, int y, float r) { 
		int cellID = index(x, y);
        if (cellID < 0 || cellID >= cells.size()) {
            throw new RuntimeException("Illegal cellID " + cellID);
        }
        Queue<Iterator<Vertex>> resultCells = new LinkedList<Iterator<Vertex>>();

        int radius = (int) Math.ceil(r);
        int maxX = x + radius;
        if (maxX >= width) maxX = width - 1; 
        int minX = x - radius;
        if (minX < 0) minX = 0;
        int maxY = y + radius;
        if (maxY >= heigth) maxY = heigth - 1;
        int minY = y - radius;
        if (minY < 0) minY = 0;
        
        int tlC = index(minX, minY);
        int trC = index(maxX, minY);
        int blC = index(minX, maxY);

        for (int i = tlC; i <= blC; i += w) {
        	for (int j = 0; j <= trC - tlC; j++) {
        		resultCells.add(cells.get(i + j).iterator());
        	}
        }
        
        return new RadiusIterable(x, y, r, resultCells); 
	}
	
	public void clear() {
		for (Queue<Vertex> inner: cells) {
			inner.clear();
		}
	}
	
	private int index(int x, int y) {
        return (int)(x * f) + (int)(y * f) * w;
	}

    private class RadiusIterable implements Iterable<Vertex>, Iterator<Vertex> {
        private Iterator<Iterator<Vertex>> cells;
        private Iterator<Vertex> cell;
        private Vertex next;
        private int x, y;
        private float r;

        public RadiusIterable(int x, int y, float r, Iterable<Iterator<Vertex>> cells) {
            this.cells = cells.iterator();
            this.cell  = this.cells.next(); // at least the base cell
        	this.x = x;
            this.y = y;
            this.r = r;
            this.next = advance();
        }

        @Override
		public Iterator<Vertex> iterator() {
			return this;
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
		
        private float distance(int x1, int y1, int x2, int y2) {
            return (float) Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
        }
		
		private Vertex advance() {
			do {
				while (cell.hasNext()) {
				  Vertex v = cell.next();
					if (distance(x, y, v.x(), v.y()) <= r) {
						return v;
					}
				}
				if (cells.hasNext()) {
					cell = cells.next();
				} else {
					break;
				}
			} while (true);
			return null;
		}
    }
}
