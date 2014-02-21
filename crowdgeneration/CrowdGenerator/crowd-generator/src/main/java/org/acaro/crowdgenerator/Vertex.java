package org.acaro.crowdgenerator;

import java.awt.Color;

public class Vertex implements Comparable<Vertex> {
	public static final int SIZE = 10;
	private int x, y;
	private int id;
	private Color color;
	
	public Vertex(int id) {
	  this(id, 0, 0);
	}
	
	public Vertex(int id, int x, int y) {
		this.id = id;
		this.x = x;
		this.y = y;
	}
	
	public Vertex(Vertex other) {
		this.id = other.id;
		this.x  = other.x;
		this.y  = other.y;
	}
	
	public int x() {
		return x;
	}
	
	public int y() {
		return y;
	}
	
	public int id() {
		return this.id;
	}

	public int x(int x) {
		int old = this.x;
		this.x = x;
		
		return old;
	}
	
	public int y(int y) {
		int old = this.y;
		this.y = y;
		
		return old;
	}
	
	public Color color() {
		return this.color;
	}
	
	public void color(Color color) {
		this.color = color;
	}
	
    public boolean overlaps(Vertex that) {
    	return (this.x() == that.x() && this.y() == that.y());
    }

	@Override
	public String toString() {
		return "v:" + id + " = (" + x + ", " + y + ")";
	}

	@Override
	public int compareTo(Vertex other) {
		if (this.id() > other.id()) {
			return 1;
		} else if (this.id() < other.id()) {
			return -1;
		} else {
			return 0;
		}
	}
	
	@Override
	public boolean equals(Object other) {
	    boolean result = false;
	    if (other instanceof Vertex) {
	        Vertex that = (Vertex) other;
	        result = (this.id() == that.id());
	    }
	    return result;
	}
	
    @Override 
    public int hashCode() {
        return (41 * (41 + this.id()));
    }
}
