package org.acaro.spectralclustering;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferStrategy;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JPanel;

import org.acaro.spectralclustering.KMeansClusterer.Centroid;

public class OpenGLVisualizer extends Canvas {
  private static final Color[] colors = {
    Color.BLUE,
    Color.CYAN,
    Color.GREEN,
    Color.MAGENTA,
    Color.ORANGE,
    Color.PINK
  };
	private static final long serialVersionUID = 1L;
	private BufferStrategy strategy;
	private int width, heigth;
	
	public OpenGLVisualizer(int width, int heigth) {
		this.width = width;
		this.heigth = heigth;
		
		JFrame container = new JFrame("Clusterer");
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
		container.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		// create the buffering strategy which will allow AWT
		// to manage our accelerated graphics
		createBufferStrategy(2);
		strategy = getBufferStrategy();
	}
	
	public void draw(double[][] data, List<Centroid> clusters) {
	  Graphics2D g = (Graphics2D) strategy.getDrawGraphics();
	  g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

	  // blank the screen
	  g.setColor(Color.WHITE);
	  g.fillRect(0, 0, width, heigth);
	  
	  int c = 0;
	  for(Centroid cluster : clusters) {
	    Color color = colors[c++];
	    g.setColor(color);
	    for (Integer p : cluster.getDataPoints()) {
	      g.fillOval((int) Math.round(data[p][0]), (int) Math.round(data[p][1]), 5, 5);
	    }
	  }
	  
	  g.dispose();
	  strategy.show();
	}
}
