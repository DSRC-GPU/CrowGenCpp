package org.acaro.crowdgenerator.visualizer;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Panel;
import java.awt.RenderingHints;
import java.awt.image.BufferStrategy;
import java.util.Collection;
import java.util.Map.Entry;

import javax.swing.JFrame;
import javax.swing.JPanel;

import org.acaro.crowdgenerator.Vertex;
import org.acaro.spectralclustering.SpectralClusteringRunner;

import com.google.common.collect.SortedSetMultimap;

public class EigenSpace extends Canvas {
	private static final long serialVersionUID = 1L;
	private BufferStrategy strategy;
	private int width, heigth, grid;
	
	public EigenSpace(int width, int heigth) {
		this(width, heigth, 0);
	}
	
	public EigenSpace(int width, int heigth, int grid) {
		this.width = width;
		this.heigth = heigth;
		this.grid = grid;
		
		JFrame container = new JFrame("Flow generator");
		JPanel panel = (JPanel) container.getContentPane();
		panel.setPreferredSize(new Dimension(width, heigth));
		panel.setLayout(null);
		
		// setup our canvas size and put it into the content of the frame
		setBounds(0, 0, width, heigth);
		panel.add(this);
		// Tell AWT not to bother repainting our canvas since we're
		// going to do that our self in accelerated mode
		setIgnoreRepaint(true);
		
		// finally make the window visible 
		container.pack();
		container.setResizable(false);
		container.setVisible(true);
		// create the buffering strategy which will allow AWT
		// to manage our accelerated graphics
		createBufferStrategy(2);
		strategy = getBufferStrategy();
	}
	
	public void initialize() { }
	
	public void draw(double[][] points) {
		Graphics2D g = (Graphics2D) strategy.getDrawGraphics();
		g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// blank the screen
		g.setColor(Color.black);
		g.fillRect(0, 0, width, heigth);
		
		double minX = Double.POSITIVE_INFINITY;
		double minY = Double.POSITIVE_INFINITY;
		double maxX = Double.NEGATIVE_INFINITY;
		double maxY = Double.NEGATIVE_INFINITY;
		for (double[] point: points) {
		  minX = Math.min(minX, point[0]);
		  minY = Math.min(minY, point[1]);
      maxX = Math.max(maxX, point[0]);
      maxY = Math.max(maxY, point[1]);
		}
		
		g.setColor(Color.white);
		for (double[] point : points) {
		  double pointX = SpectralClusteringRunner.translate(point[0], maxX, minX, getWidth(), 0);
		  double pointY = SpectralClusteringRunner.translate(point[1], maxY, minY, getHeight(), 0);
		  g.fillOval((int) Math.round(pointX), (int) Math.round(pointY), 10, 10);
		}
		
		// finally, we've completed drawing so clear up the graphics
		// and flip the buffer over
		g.dispose();
		strategy.show();
	}
}
