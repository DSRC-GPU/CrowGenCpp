
#ifndef POINT_HPP
#define POINT_HPP

class Point
{
  private:
    int _x, _y;

  public:
    Point();
    Point(int, int);
    int x() const;
    int x(int);
    int y() const;
    int y(int);
};

#endif
