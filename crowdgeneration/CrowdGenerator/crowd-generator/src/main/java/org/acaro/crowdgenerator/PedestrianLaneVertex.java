package org.acaro.crowdgenerator;

import java.awt.Color;

public class PedestrianLaneVertex extends Vertex {
  private int lane = -1;

  public static Color convertToColor(int lane) {
    Color col;
    
    switch (lane) {
      case 0:  col = Color.red;       break;
      case 1:  col = Color.blue;      break;
      case 2:  col = Color.green;     break;
      case 3:  col = Color.darkGray;  break;
      case 4:  col = Color.cyan;      break;
      case 5:  col = Color.gray;      break;
      case 6:  col = Color.lightGray; break;
      case 7:  col = Color.magenta;   break;
      case 8:  col = Color.orange;    break;
      case 9:  col = Color.pink;      break;
      case 10: col = Color.yellow;    break;
      default: col = Color.white;
    }
    
    return col;
  }
  
  public PedestrianLaneVertex(int id, int x, int y) {
    super(id, x, y);
  }
  
  public PedestrianLaneVertex(PedestrianLaneVertex other) {
    super(other);
    setLane(other.getLane());
  }
  
  public int getLane() {
    return this.lane;
  }
  
  public void setLane(int lane) {
    this.lane = lane;
    color(convertToColor(lane));
  }
}
