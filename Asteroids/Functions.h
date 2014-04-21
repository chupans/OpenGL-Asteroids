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
}