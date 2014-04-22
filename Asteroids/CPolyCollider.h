#pragma once
#include "math_3d.h"
#include <vector>
#include "ICollider.h"



class TEdge
{
public:
  TEdge (Vector2f startPoint, Vector2f endPoint);
  void Rotate(Vector2f pivotPoint, float angle);
  float onWhichSide(Vector2f _point);
  Vector2f getEdgeVector();
  void UpdatePosition (Vector2f shift);

  Vector2f _normal;
  Vector2f _p1, _p2;
};

class CPolyCollider : public ICollider
{
public:
  CPolyCollider(Vector2f points[], unsigned int count, Vector2f position);
  virtual bool ContainPoint (Vector2f point);
  virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint);
  virtual bool DoCollideWith (ICollider *other);
  virtual Vector2f GetCollissionVector(ICollider *other);
  virtual void UpdatePosition(Vector2f shift);
  virtual void Rotate( float angle, float delta );
  virtual bool ProbablyCollideWith (ICollider *other);
  virtual float GetCircumCircleRadius();
  virtual Vector2f GetPosition();

  std::vector<TEdge> _edges; 
};
