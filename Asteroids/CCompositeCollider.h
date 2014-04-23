#pragma once
#include "ICollider.h"
#include "CPolyCollider.h"

class CCompositeCollider : public ICollider
{
public:
  CCompositeCollider(Vector2f **points, int count[], int objCount, Vector2f position);
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

  std::vector<CPolyCollider*> m_polyColliders;

};
