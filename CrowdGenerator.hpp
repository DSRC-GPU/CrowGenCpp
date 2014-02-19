
class CrowdGenerator
{
  public:
    CrowdGenerator();

    // Generates vertices and places them randomly in the given crowd. The
    // number of vertices generated is equal to the population specified in the
    // given MoveMap.
    void populate(Crowd&, MoveMap&) const;
};
