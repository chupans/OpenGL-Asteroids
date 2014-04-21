#pragma once
#include "ICollider.h"

class TBoundaryCollider : ICollider
{
public:
  TBoundaryCollider(Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideCollider);
  ~TBoundaryCollider();

  virtual void Rotate( float angle, float delta );
  virtual void UpdatePosition( Vector2f shift );
  virtual bool DoCollideWith( ICollider *other );
  virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint);
  virtual Vector2f GetCollissionVector( ICollider *other );
  virtual bool IsPointInside( Vector2f point );

private:
  Vector2f m_p1, m_p2;
  int m_crossProductFactor;

};
