#pragma once
#include "math_3d.h"

class ICollider
{
  public:
    virtual bool IsPointInside (Vector2f point) = 0;
    virtual bool DoCollideWith (ICollider *other, Vector2f &collidePoint) = 0;
    virtual bool DoCollideWith (ICollider *other) = 0;
    virtual Vector2f GetCollissionVector (ICollider *other) = 0;  //HACK нужен ли другой?
    virtual void UpdatePosition (Vector2f shift) = 0;
    virtual void Rotate (float angle, float delta) = 0;
};