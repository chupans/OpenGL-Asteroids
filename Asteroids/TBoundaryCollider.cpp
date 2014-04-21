#include "TBoundaryCollider.h"

TBoundaryCollider::TBoundaryCollider(Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideCollider)
{
  m_p1 = startPoint;
  m_p2 = endPoint;
  m_crossProductFactor = 1;
  if (!IsPointInside(pointInsideCollider))
    m_crossProductFactor = -1;
}

TBoundaryCollider::~TBoundaryCollider(void)
{
}

void TBoundaryCollider::Rotate( float angle, float delta )
{
  return;
}

void TBoundaryCollider::UpdatePosition( Vector2f shift )
{
  return;
}

bool TBoundaryCollider::DoCollideWith( ICollider *other )
{
  return false;
}

bool TBoundaryCollider::DoCollideWith( ICollider *other, Vector2f &collidePoint )
{
  return false;
}

Vector2f TBoundaryCollider::GetCollissionVector( ICollider *other )
{
  Vector2f boundaryVector = m_p2 - m_p1;
  Vector2f collVector(-boundaryVector.getY(), boundaryVector.getX());
  collVector.normalize();
  return collVector;

}

// < 0 слева. z координата векторного произведения
bool TBoundaryCollider::IsPointInside( Vector2f point )
{
  Vector2f a = m_p2 - m_p1;
  Vector2f b(point.getX() - m_p2.getX(), point.getY() - m_p2.getY());

  return (m_crossProductFactor * (a.getX() * b.getY() - a.getY() * b.getX())) < 0;
}
