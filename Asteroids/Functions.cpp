#include "Functions.h"
#include <vector>
#include <algorithm>
#include "time.h"

using namespace std;

void GenerateConvexPolygon(int count, Vector2f resultPoints[])
{
  vector<float> angles(count, 0);
  int i;
  float s, summ, factor;
  float x, y;
  for (i = 0, summ = 0; i < count; i++)
  {
    angles[i] = (float)(rand() % 50 + 25)/ 100.0f;  
    summ += angles[i];
  }
  factor = 2 * M_PI / summ;

  summ = 0;
  for (i = 0; i < count; i++)
  {
    angles[i] *= factor;
    summ += angles[i];
  }

  float length = 0.2;
  for (i = 0, summ = 0; i < count; i++)
  {
    length += (float)(rand()% 20)/1000.0f - 0.01f;
    summ += angles[i];
    x = cosf(summ) * length;
    y = sinf(summ) * length;
    resultPoints[i] = Vector2f(x, y);
  }
}