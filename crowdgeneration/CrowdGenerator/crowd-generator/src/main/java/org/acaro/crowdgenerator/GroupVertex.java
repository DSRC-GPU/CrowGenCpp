package org.acaro.crowdgenerator;

public class GroupVertex extends Vertex {
	private boolean isGroup;
	public enum State { EAST, WEST, NORTH, SOUTH };
	private State state = State.SOUTH;
	
	public GroupVertex(int id, int x, int y) {
		super(id, x, y);
	}
	
	public GroupVertex(GroupVertex other) {
		super(other);
		this.isGroup = other.isGroup;
	}
	
	public boolean isGroup() {
		return this.isGroup;
	}
	
	public boolean isGroup(boolean value) {
		boolean oldValue = isGroup;
		this.isGroup = value;
		
		return oldValue;
	}
	
	public State getState() {
		return this.state;
	}
	
	public State setState(State newState) {
		State oldState = state;
		this.state = newState;
		
		return oldState;
	}
}
