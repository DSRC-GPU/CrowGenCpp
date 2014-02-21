package org.acaro.crowdgenerator;

public class LaneVertex extends Vertex {
	private boolean isFlow;
	
	public LaneVertex(int id, int x, int y) {
		super(id, x, y);
	}
	
	public LaneVertex(LaneVertex other) {
		super(other);
		this.isFlow = other.isFlow;
	}
	
	public boolean isFlow() {
		return this.isFlow;
	}
	
	public boolean isFlow(boolean value) {
		boolean oldValue = isFlow;
		isFlow = value;
		
		return oldValue;
	}
}
