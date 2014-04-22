#pragma once
#include "ICollider.h"
#include "CPolyCollider.h"

class CCompositeCollider : public ICollider
{
public:
  CCompositeCollider(std::vector<CPolyCollider*> colliders);
  ~CCompositeCollider(void);

  virtual void Rotate( float angle, float delta );
  virtual void UpdatePosition( Vector2f shift );
  virtual bool DoCollideWith( ICollider *other );
  virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint);
  virtual Vector2f GetCollissionVector( ICollider *other );
  virtual bool ContainPoint( Vector2f point );
  virtual Vector2f GetPosition();
  virtual bool ProbablyCollideWith( ICollider *other );
  virtual float GetCircumCircleRadius();

private:
  std::vector<CPolyCollider*> m_polyColliders;

};
