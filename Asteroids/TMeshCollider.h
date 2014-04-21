#pragma once
#include "math_3d.h"
#include <vector>
#include "ICollider.h"

using namespace std;

enum COLLIDER_TYPE
{
  COLLIDER_CIRCLE,
  COLLIDER_MESH
};

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

class TMeshCollider : ICollider
{
public:
  TMeshCollider(Vector2f points[], unsigned int count, Vector2f center);
  virtual bool IsPointInside (Vector2f point);
  virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint);
  virtual bool DoCollideWith (ICollider *other);
  virtual Vector2f GetCollissionVector(ICollider *other);
  virtual void UpdatePosition(Vector2f shift);
  virtual void Rotate( float angle, float delta );

  vector<TEdge> _edges;
  Vector2f m_center;  
};
