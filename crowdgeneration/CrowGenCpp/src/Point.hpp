
#ifndef POINT_HPP
#define POINT_HPP

// The Point class represents a point in 2D space.
class Point
{
  private:
    float _x, _y;

  public:
    Point();
    Point(float, float);
    float x() const;
    float x(float);
    float y() const;
    float y(float);
    bool closeTo(Point&, int) const;
    bool operator==(const Point&);
    bool operator!=(const Point&);
};

#endif
