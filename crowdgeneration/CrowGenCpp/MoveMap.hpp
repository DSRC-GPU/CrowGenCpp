
#ifndef MOVEMAP_HPP
#define MOVEMAP_HPP

class MoveMap
{
  private:
    int mm_width, mm_height;
    int mm_population;

    // Dynamically allocated 2d array used to store char map.
    int **mm_charmap;

  public:
    MoveMap(int p, int w, int h);

    // Gives the direction (1-9 a la numpad) the crowd is moving in, at the
    // given coordinates on the map.
    int getCrowdDirection(int x, int y) const;

    int height() const;
    int height(int h);
    int width() const;
    int width(int w);
    int population() const;
    int population(int p);
};

#endif
