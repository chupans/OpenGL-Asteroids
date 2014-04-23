#include "CCollision.h"



CCollision::CCollision( IBody *body1, IBody *body2 )
{
  m_body1 = body1;
  m_body2 = body2;
  stepsLeftIdle = 0;
  m_done = false;
  startDistance = 0.0f;
}

bool CCollision::operator==( CCollision const &other )
{
  return (m_body1 == other.m_body1 && m_body2 == other.m_body2) || 
    (m_body1 == other.m_body2 && m_body2 == other.m_body1);
}

bool CCollision::ProcessCollision( float deltaTime )
{
  bool collide;
  if (stepsLeftIdle > 0)
  {
    if (startDistance > Vector2f::distanceBetween(m_body2->GetPosition(), m_body1->GetPosition()))
    {
      if (!m_body1->IsStatic() && !m_body2->IsStatic())
      {
        int dg;
        dg = 3;
      }
    }
    collide = false;
    collide |= m_body1->GetCollider()->DoCollideWith(m_body2->GetCollider());
    collide |= m_body2->GetCollider()->DoCollideWith(m_body1->GetCollider());
    if (!collide)
    {
      m_done = true;
      return true;
    }
    //if (Vector2f::distanceBetween(m_body1->GetPosition(), m_body2->GetPosition()) > 0);
    stepsLeftIdle--;
    if (stepsLeftIdle == 0)
    {
      m_done = true;
      return true;
    }
    else
      return false;
  }
  else
  {
    Vector2f collidePoint = Vector2f::ZERO;
    Vector2f collidePoint2 = Vector2f::ZERO;
    collide = false;
    collide |= m_body1->GetCollider()->DoCollideWith(m_body2->GetCollider(), collidePoint);
    collide |= m_body2->GetCollider()->DoCollideWith(m_body1->GetCollider(), collidePoint2);
    collidePoint += collidePoint2;
    collidePoint/=2;
    if (collide)
    {
      Vector2f v2 = m_body2->GetSpeed();
      Vector2f v1 = m_body1->GetSpeed();
      Vector2f v1temp = v1;
      Vector2f v2temp = v2;
      Vector2f newV1 = Vector2f::ZERO;
      Vector2f newV2 = Vector2f::ZERO;

      float m1 = m_body1->GetMass();
      float m2 = m_body2->GetMass();
      if (m_body2->IsStatic())
      {
        Vector2f reaction = m_body2->GetCollider()->GetCollissionVector(m_body1->GetCollider());
        newV1 = v1;
        // HACK: только бандуари pod прямым углом
        if (reaction.getY() == 0)
        {
          newV1.setX(-1 * m_body1->GetSpeed().getX());
          m_body1->SetSpeedChange(newV1 - v1);
        }
        else
        {
          newV1.setY(-1 * m_body1->GetSpeed().getY());
          m_body1->SetSpeedChange(newV1 - v1);
        }
        //m_body1->AddToAngularSpeed(-2*(m_body1->GetAngularSpeed()));
      }
      else
      {
        float angleFactor, momentumArmLength, direction;
        Vector2f obj1Arm, obj2Arm;
        direction = Vector2f::zCrossProduct(m_body2->GetPosition(), m_body2->GetPosition() + v2, collidePoint);
        if (direction != 0)
        {
          direction = direction > 0 ? 1 : -1;
          obj1Arm = m_body1->GetPosition() - collidePoint;
          momentumArmLength = obj1Arm.GetLength();
          obj1Arm.normalize();
          v2temp.normalize();
          angleFactor = 1.0f - fabs(obj1Arm.dotProduct(v2temp)); //max when angle between vectors is 90
          m_body1->SetAngularSpeedChange(-4*momentumArmLength * (m2/m1) * angleFactor * direction);
        }

        direction = Vector2f::zCrossProduct(m_body1->GetPosition(), m_body1->GetPosition() + v1, collidePoint);
        if (direction != 0)
        {
          direction = direction > 0 ? 1 : -1;
          obj2Arm = m_body2->GetPosition() - collidePoint;
          momentumArmLength = obj2Arm.GetLength();
          obj2Arm.normalize();
          v1temp.normalize();
          angleFactor = 1.0f - fabs(obj2Arm.dotProduct(v1temp)); //max when angle between vectors is 90
          m_body2->SetAngularSpeedChange(-4*momentumArmLength * (m1/m2) * angleFactor * direction);
        }

        newV2 = ( m1 * v1 + m2 * v2 + (v1 - v2) * m1) / (m1 + m2);
        newV1 = newV2 - v1 + v2;

        m_body1->SetSpeedChange(newV1 - v1);
        m_body2->SetSpeedChange(newV2 - v2);
      }
      startDistance = Vector2f::distanceBetween(m_body2->GetPosition(), m_body1->GetPosition());
      stepsLeftIdle = 50;
    }
    return false;
  }
}
