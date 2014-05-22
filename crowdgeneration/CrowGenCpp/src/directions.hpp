
#include <vector>

using namespace std;

// This namespace is used to store the constant direction arrays. These arrays
// map the direction numbers (0-9) to actual coordinate changes (-1,0,1) for
// both the X and Y directions.
namespace directions
{
  extern const vector<float> DIRS_1_X;
  extern const vector<float> DIRS_2_X;
  extern const vector<float> DIRS_3_X;
  extern const vector<float> DIRS_4_X;
  extern const vector<float> DIRS_5_X;
  extern const vector<float> DIRS_6_X;
  extern const vector<float> DIRS_7_X;
  extern const vector<float> DIRS_8_X;
  extern const vector<float> DIRS_9_X;
  extern const vector<float> DIRS_1_Y;
  extern const vector<float> DIRS_2_Y;
  extern const vector<float> DIRS_3_Y;
  extern const vector<float> DIRS_4_Y;
  extern const vector<float> DIRS_5_Y;
  extern const vector<float> DIRS_6_Y;
  extern const vector<float> DIRS_7_Y;
  extern const vector<float> DIRS_8_Y;
  extern const vector<float> DIRS_9_Y;

  extern const vector<float> DIRS_X[10];
  extern const vector<float> DIRS_Y[10];

  extern const int numDirs;
}
