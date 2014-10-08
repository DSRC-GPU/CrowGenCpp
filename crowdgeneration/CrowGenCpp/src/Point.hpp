
#ifndef POINT_HPP
#define POINT_HPP

// The Point class represents a point in 2D space.
class Point
{
  private:
    int _x, _y;

  public:
    Point();
    Point(int, int);
    Point(const Point&);
    int x() const;
    int x(int);
    int y() const;
    int y(int);
    bool closeTo(const Point&, int) const;
    bool operator==(const Point&);
    bool operator!=(const Point&);
};

#endif
