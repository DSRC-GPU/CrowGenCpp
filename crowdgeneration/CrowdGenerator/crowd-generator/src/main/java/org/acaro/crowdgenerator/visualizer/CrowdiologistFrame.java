package org.acaro.crowdgenerator.visualizer;

import java.awt.Dimension;
import java.awt.BorderLayout;

import javax.swing.JFrame;

public class CrowdiologistFrame extends JFrame {
  private NewOpenGLVisualizer simulatorPanel;
  private HistogramPanel histogramPanel;
  
  public CrowdiologistFrame(int simW, int simH) {
    setTitle("Crowdiologist 1.0");
    setSize(simW, simH * 2);
    setLayout(new BorderLayout()); // Set the layout
    setLocationRelativeTo(null);  // Center the frame
    setPreferredSize(new Dimension(simW, simH * 2));
    setResizable(true);
    setVisible(true);
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    simulatorPanel = new NewOpenGLVisualizer(simW, simH, 10);
    histogramPanel = new HistogramPanel(simW, simH);

    add(simulatorPanel);
    add(histogramPanel);
    
    simulatorPanel.initialize();
    histogramPanel.initialize();
    
    pack();
  }
  
  public NewOpenGLVisualizer getSimulatorPanel() {
    return simulatorPanel;
  }
  
  public HistogramPanel getHistogramPanel() {
    return histogramPanel;
  }
}
