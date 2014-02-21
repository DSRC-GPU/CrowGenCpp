package org.acaro.crowdgenerator.visualizer;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferStrategy;
import java.util.Collections;
import java.util.List;

import com.google.common.collect.Lists;

public class HistogramPanel extends Canvas {
  private static final long serialVersionUID = 1L;
  private BufferStrategy strategy;
  
  public HistogramPanel(int width, int height) {
    setBounds(0, 0, width, height);
    setIgnoreRepaint(true);
  }
  
  public void initialize() {
    // create the buffering strategy which will allow AWT
    // to manage our accelerated graphics
    createBufferStrategy(2);
    strategy = getBufferStrategy();
  }
  
  public void draw(double[] values, boolean sort) {
    Graphics2D g = (Graphics2D) strategy.getDrawGraphics();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    List<Double> v = Lists.newArrayListWithCapacity(values.length);
    double maxValue = - Double.MAX_VALUE;
    for (int i = 0; i < values.length; i++) {
      v.add(values[i]);
      maxValue = Math.max(maxValue, values[i]);
    }
    if (sort) {
      Collections.sort(v, Collections.reverseOrder());
    }
    System.out.println(v);
    int width = getWidth();
    int height = getHeight();

    g.setColor(Color.white);
    g.fillRect(0, 0, width, height);
    int barWidth = (int) Math.floor((float) width / (float) v.size());
    int xPos = 0;
    g.setColor(Color.blue);
    for (Double value : v) {
      int barHeight = (int) Math.round((value / maxValue) * (height/2.2));
      int yPos = height - barHeight;
      Rectangle2D bar = new Rectangle2D.Float(xPos, yPos, barWidth, barHeight);
      g.fill(bar);
      g.draw(bar);
      xPos += barWidth;
    }
    g.dispose();
    strategy.show();
  }
}
