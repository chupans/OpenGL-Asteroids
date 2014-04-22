#include "CCollision.h"



CCollision::CCollision( IBody *body1, IBody *body2 )
{
  m_body1 = body1;
  m_body2 = body2;
}

bool CCollision::operator==( CCollision const &other )
{
  return (m_body1 == other.m_body1 && m_body2 == other.m_body2) || 
    (m_body1 == other.m_body2 && m_body2 == other.m_body1);
}

void CCollision::ProcessCollision( float deltaTime )
{
  if (stepsLeftIdle != 0)
  {
    if (Vector2f::distanceBetween(m_body1->GetPosition(), m_body2->GetPosition()) > 0);
    stepsLeftIdle--;
    return;
  }



}
