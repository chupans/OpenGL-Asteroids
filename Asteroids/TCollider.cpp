#include "TCollider.h"

bool TCollider::isPointInside( Vector2f point )
{
  switch (m_colliderType)
  {
  case COLLIDER_CIRCLE:
    float distance;
    distance = Vector2f::squareDistanceBetween(m_center, point);
    if (distance < m_radius * m_radius)
      return true;
    else
      return false;
    break;
  }
}

bool TCollider::doCollideWith( TCollider other )
{
  switch (m_colliderType)
  {
  case COLLIDER_CIRCLE:
    float distance;
    distance = Vector2f::squareDistanceBetween(this->m_center, other.m_center);
    distance -= this->m_radius * this->m_radius + other.m_radius * other.m_radius;
    if (distance > 0)
      return false;
    else
      return true;
    break;
  }
}

float TCollider::getCollisionLength( TCollider other )
{
  float distance;
  distance = Vector2f::squareDistanceBetween(this->m_center, other.m_center);
  distance -= this->m_radius * this->m_radius + other.m_radius * other.m_radius;
  return fabs(distance);
}
