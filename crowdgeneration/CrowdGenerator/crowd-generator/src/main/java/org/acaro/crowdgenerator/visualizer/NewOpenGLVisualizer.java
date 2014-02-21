package org.acaro.crowdgenerator.visualizer;

import java.awt.BasicStroke;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Panel;
import java.awt.RenderingHints;
import java.awt.image.BufferStrategy;
import java.util.Map.Entry;

import javax.swing.JFrame;
import javax.swing.JPanel;

import org.acaro.crowdgenerator.Edge;
import org.acaro.crowdgenerator.Vertex;

import com.google.common.collect.SortedSetMultimap;

public class NewOpenGLVisualizer extends Canvas {
	private static final long serialVersionUID = 1L;
	private static final int MAX_EDGE = 5;
	private BufferStrategy strategy;
	private int width, heigth, grid;
	
	public NewOpenGLVisualizer(int width, int heigth) {
		this(width, heigth, 0);
	}
	
	public NewOpenGLVisualizer(int width, int heigth, int grid) {
		this.width = width;
		this.heigth = heigth;
		this.grid = grid;
		
		// setup our canvas size and put it into the content of the frame
		setBounds(0, 0, width, heigth);
		// Tell AWT not to bother repainting our canvas since we're
		// going to do that our self in accelerated mode
		setIgnoreRepaint(true);
	}
	
	public void initialize() {
    // create the buffering strategy which will allow AWT
    // to manage our accelerated graphics
    createBufferStrategy(2);
    strategy = getBufferStrategy();
	}
	
	public void draw(Vertex[] vertices, SortedSetMultimap<Vertex,Edge> edges) {
		draw(vertices, edges, false);
	}
	
	public void draw(Vertex[] vertices, SortedSetMultimap<Vertex,Edge> edges, boolean drawGrid) {
		Graphics2D g = (Graphics2D) strategy.getDrawGraphics();
		g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// blank the screen
		g.setColor(Color.black);
		g.fillRect(0, 0, width, heigth);
		
		// draw edges
		g.setColor(Color.white);
		for (Entry<Vertex, Edge> v: edges.entries()) {
		  Vertex src = v.getKey();
		  Edge edge = v.getValue();
		  Vertex dst = edge.getDst();
		  double weight = edge.getWeight();
		  g.setStroke(new BasicStroke((int) Math.round(weight * MAX_EDGE)));
			g.drawLine(src.x(), src.y(), dst.x(), dst.y());
		}

		// draw vertices
		for (Vertex v: vertices) {
			g.setColor(v.color());
			//g.drawOval(v.x()-Vertex.SIZE, v.y()-Vertex.SIZE, Vertex.SIZE*2, Vertex.SIZE*2);
	    g.fillOval(v.x()-Vertex.SIZE, v.y()-Vertex.SIZE, Vertex.SIZE*2, Vertex.SIZE*2);
		}

		if (drawGrid) {
			if (grid == 0) {
				throw new RuntimeException("drawing grid with undefined size!");
			}
			// draw grid
			g.setColor(Color.red);
			for (int i = 0; i < heigth/grid; i++) {
				g.drawLine(i*10, 0, i*10, heigth);
			}
			for (int i = 0; i < width/grid; i++) {
				g.drawLine(0, i*10, width, i*10);
			}
		}
		
		// finally, we've completed drawing so clear up the graphics
		// and flip the buffer over
		g.dispose();
		strategy.show();
	}
}
