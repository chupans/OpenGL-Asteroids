#pragma once
#include "math_3d.h"
#include <vector>

template <typename T>
T Clamp (T const val, T const lowerBound, T const topBound)
{
  T clampedVal = val;
  T betweenBounds = topBound - lowerBound;
  while (clampedVal > topBound)
    clampedVal -= betweenBounds;
  while (clampedVal < lowerBound)
    clampedVal += betweenBounds;

  return clampedVal;
};

class PolyFunctions
{
public:
  static void GenerateConvexPolygon(int count, std::vector<Vector2f> &resultPoints);
  static void CutPolygonIntoPeaces(std::vector<Vector2f> &polygonPoints, std::vector< std::vector<Vector2f> > &polyPieces);
  static float CalcPolyArea(Vector2f points[], int count);
};