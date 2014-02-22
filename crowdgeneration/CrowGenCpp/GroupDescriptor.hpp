
#ifndef MOVEMAP_HPP
#define MOVEMAP_HPP

class GroupDescriptor
{
  private:
    int _width, _height;
    int _population;

    // Dynamically allocated 2d array used to store char map.
    int **_charmap;

  public:
    GroupDescriptor(int p, int w, int h);
    GroupDescriptor();

    // Gives the direction (1-9 a la numpad) the crowd is moving in, at the
    // given coordinates on the map.
    int getCrowdDirection(int x, int y) const;

    int height() const;
    int height(int h);
    int width() const;
    int width(int w);
    int population() const;
    int population(int p);
    char* charmap() const;
    char* charmap(char*);
};

#endif
