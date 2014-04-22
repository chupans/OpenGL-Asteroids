#include "CBoundaryCollider.h"

CBoundaryCollider::CBoundaryCollider(Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideCollider)
{
  m_p1 = startPoint;
  m_p2 = endPoint;
  m_center = m_p2 - m_p1;
  m_CircumCircleRadius = Vector2f::distanceBetween(m_p2, m_p1) * 10;  //HACK
  m_crossProductFactor = 1;
  if (!ContainPoint(pointInsideCollider))
    m_crossProductFactor = -1;
}

CBoundaryCollider::~CBoundaryCollider(void)
{
}

void CBoundaryCollider::Rotate( float angle, float delta )
{
  return;
}

void CBoundaryCollider::UpdatePosition( Vector2f shift )
{
  return;
}

bool CBoundaryCollider::DoCollideWith( ICollider *other )
{
  return false;
}

bool CBoundaryCollider::DoCollideWith( ICollider *other, Vector2f &collidePoint )
{
  return false;
}

Vector2f CBoundaryCollider::GetCollissionVector( ICollider *other )
{
  Vector2f boundaryVector = m_p2 - m_p1;
  Vector2f collVector(-boundaryVector.getY(), boundaryVector.getX());
  collVector.normalize();
  return collVector;

}

// < 0 слева. z координата векторного произведения
bool CBoundaryCollider::ContainPoint( Vector2f point )
{
  Vector2f a = m_p2 - m_p1;
  Vector2f b(point.getX() - m_p2.getX(), point.getY() - m_p2.getY());

  return (m_crossProductFactor * (a.getX() * b.getY() - a.getY() * b.getX())) < 0;
}

Vector2f CBoundaryCollider::GetPosition()
{
  return m_center;
}

bool CBoundaryCollider::ProbablyCollideWith( ICollider *other )
{
  return true;//HACK
}

float CBoundaryCollider::GetCircumCircleRadius()
{
  return m_CircumCircleRadius;
}
