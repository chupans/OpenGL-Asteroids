#include "CCompositeCollider.h"
using namespace std;

CCompositeCollider::CCompositeCollider(vector<CPolyCollider*> colliders)
{
  vector<CPolyCollider*>::iterator colliderIt;
  float distance;
  m_center = Vector2f::ZERO;
  m_CircumCircleRadius = 0;

  for (colliderIt = colliders.begin(); colliderIt < colliders.end(); colliderIt++)
  {
    m_polyColliders.push_back(*colliderIt);
    m_center += (*colliderIt)->GetPosition() / colliders.size();
  }

  for (colliderIt = colliders.begin(); colliderIt < colliders.end(); colliderIt++)
  {
    distance = Vector2f::distanceBetween(m_center, (*colliderIt)->GetPosition()) + (*colliderIt)->GetCircumCircleRadius();
    if (distance > m_CircumCircleRadius)
      m_CircumCircleRadius = distance;
  }
}

CCompositeCollider::~CCompositeCollider(void)
{
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    delete *colliderIt;
  }
}
void CCompositeCollider::Rotate( float angle, float delta )
{
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    (*colliderIt)->Rotate(angle, delta);
  }
}

void CCompositeCollider::UpdatePosition( Vector2f shift )
{
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    (*colliderIt)->UpdatePosition(shift);
  }
}

bool CCompositeCollider::DoCollideWith( ICollider *other )
{
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    if ((*colliderIt)->DoCollideWith(other))
      return true;
  }
  return false;
}

bool CCompositeCollider::DoCollideWith( ICollider *other, Vector2f &collidePoint )
{
  bool doCollide = false;
  Vector2f temp = Vector2f::ZERO;
  collidePoint = Vector2f::ZERO;
  int collidePointsCount = 0;
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    if ((*colliderIt)->DoCollideWith(other, temp))
    {
      doCollide = true;
      collidePoint += temp;
      collidePointsCount++;
    }
  }
  if (collidePointsCount > 0)
    collidePoint /= collidePointsCount;

  return doCollide;
}

Vector2f CCompositeCollider::GetCollissionVector( ICollider *other )
{
  //FIXME
  return Vector2f::ZERO;
}

bool CCompositeCollider::ContainPoint( Vector2f point )
{
  bool doCollide = false;
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    if ((*colliderIt)->ContainPoint(point))
      return true;
  }
  return false;
}

Vector2f CCompositeCollider::GetPosition()
{
  return m_center;
}

bool CCompositeCollider::ProbablyCollideWith( ICollider *other )
{
  vector<CPolyCollider*>::iterator colliderIt;
  for (colliderIt = m_polyColliders.begin(); colliderIt < m_polyColliders.end(); colliderIt++)
  {
    if ((*colliderIt)->ProbablyCollideWith(other))
      return true;
  }
  return false;
}

float CCompositeCollider::GetCircumCircleRadius()
{
  return m_CircumCircleRadius;
}
