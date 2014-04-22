#pragma once
#include "ICollider.h"

class CBoundaryCollider : ICollider
{
public:
  CBoundaryCollider(Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideCollider);
  ~CBoundaryCollider();

  virtual void Rotate( float angle, float delta );
  virtual void UpdatePosition( Vector2f shift );
  virtual bool DoCollideWith( ICollider *other );
  virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint);
  virtual Vector2f GetCollissionVector( ICollider *other );
  virtual bool ContainPoint( Vector2f point );
  virtual Vector2f GetPosition();
  virtual bool ProbablyCollideWith (ICollider *other);
  virtual float GetCircumCircleRadius();

private:
  Vector2f m_p1, m_p2;
  int m_crossProductFactor;

};
