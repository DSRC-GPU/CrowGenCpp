
#include "directions.hpp"

namespace directions
{
  const vector<float> DIRS_1_X = { -0.705, 0 };
  const vector<float> DIRS_2_X = { -0.25, 0.25 };
  const vector<float> DIRS_3_X = { 0, 0.705 };
  const vector<float> DIRS_4_X = { -1, 0 };
  const vector<float> DIRS_5_X = { -0.25, 0.25 };
//  const vector<float> DIRS_5_X = { 0, 0 };
  const vector<float> DIRS_6_X = { 0, 1 };
  const vector<float> DIRS_7_X = { -0.705, 0};
  const vector<float> DIRS_8_X = { -0.25, 0.25 };
  const vector<float> DIRS_9_X = { 0, 0.705 };
  const vector<float> DIRS_1_Y = { 0, 0.705 };
  const vector<float> DIRS_2_Y = { 0, 1 };
  const vector<float> DIRS_3_Y = { 0, 0.705 };
  const vector<float> DIRS_4_Y = { -0.25, 0.25 };
  const vector<float> DIRS_5_Y = { -0.25, 0.25 };
//const vector<float> DIRS_5_Y = { 0, 0 };
  const vector<float> DIRS_6_Y = { -0.25, 0.25 };
  const vector<float> DIRS_7_Y = { -0.705, 0 };
  const vector<float> DIRS_8_Y = { -1, 0 };
  const vector<float> DIRS_9_Y = { -0.705, 0 };

  const vector<float> DIRS_X[10] = {
    DIRS_1_X,
    DIRS_2_X,
    DIRS_3_X,
    DIRS_4_X,
    DIRS_5_X,
    DIRS_6_X,
    DIRS_7_X,
    DIRS_8_X,
    DIRS_9_X
  };

  const vector<float> DIRS_Y[10] = {
    DIRS_1_Y,
    DIRS_2_Y,
    DIRS_3_Y,
    DIRS_4_Y,
    DIRS_5_Y,
    DIRS_6_Y,
    DIRS_7_Y,
    DIRS_8_Y,
    DIRS_9_Y
  };

  const int numDirs = 10;
}
