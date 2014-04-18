#pragma once
#include "math_3d.h"

enum COLLIDER_TYPE
{
  COLLIDER_CIRCLE,
  COLLIDER_BOX
};

class TCollider
{
public:
  virtual bool isPointInside( Vector2f point );
  virtual bool doCollideWith( TCollider other );
   float getCollisionLength( TCollider other );

public:
  COLLIDER_TYPE m_colliderType;
  float m_radius;
  Vector2f m_center;
};
