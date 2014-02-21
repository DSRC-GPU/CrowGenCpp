package org.acaro.crowdgenerator.indexing;

public class Interval2D {
  public Interval intervalX;
  public Interval intervalY;
  
  public Interval2D(Interval intX, Interval intY) {
    this.intervalX = intX;
    this.intervalY = intY;
  }

  public boolean contains(int x, int y) {
    return x <= intervalX.high && x >= intervalX.low &&
        y <= intervalY.high && y >= intervalY.low;
  }
}
