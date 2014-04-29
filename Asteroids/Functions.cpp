#include "Functions.h"
#include <vector>
#include <algorithm>
#include "time.h"
#include "TConstants.h"

using namespace std;

void PolyFunctions::GenerateConvexPolygon( int count, std::vector<Vector2f> &resultPoints )
{
  vector<float> angles(count, 0);
  int i;
  float summ, factor;
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

  float length = 0.1f;
  for (i = 0, summ = 0; i < count; i++)
  {
    length += (float)(rand()% 20)/1000.0f - 0.01f;
    summ += angles[i];
    x = cosf(summ) * length;
    y = sinf(summ) * length;
    resultPoints.push_back(Vector2f(x, y));
  }
}

float PolyFunctions::CalcPolyArea( Vector2f points[], int count )
{
  float area;
  int i;
  for (area = 0, i = 0; i < count; i++)
  {
    if (i+1 < count)
    {
      area += (points[i].getX() + points[i+1].getX()) * (points[i].getY() - points[i+1].getY());
    }
    else
    {
      area += (points[i].getX() + points[0].getX()) * (points[i].getY() - points[0].getY());
    }
  }
  return area;
}

void PolyFunctions::CutPolygonIntoPeaces( std::vector<Vector2f> &polygonPoints, std::vector< std::vector<Vector2f> > &polyPieces )
{
  vector<Vector2f>::iterator polyPointsIt;
  int piecesCount = polygonPoints.size() / TConstants::piecesPointCount;
  int i, j, currentPoint;
  Vector2f center = Vector2f::ZERO;

  for (polyPointsIt = polygonPoints.begin(); polyPointsIt != polygonPoints.end(); polyPointsIt++)
  {
    center += *polyPointsIt;
  }
  center /= (float)polygonPoints.size();

  vector<Vector2f> peice;
  for (i = 0, currentPoint = 0; i < piecesCount - 1; i ++)
  {
    peice.clear();
    for (j = 0; j < TConstants::piecesPointCount; j++)
    {
      peice.push_back(polygonPoints[currentPoint]);
    }
    peice.push_back(center);
    polyPieces.push_back(peice);
  }
}
