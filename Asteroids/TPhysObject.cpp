#include "TPhysObject.h"

void TPhysObject::updatePosition( float deltaTime )
{
  Vector2f shift;
  shift = speed;
  shift *= deltaTime;
  this->center += shift;
  this->m_collider.UpdatePosition(shift);
  this->m_collider.m_center = this->center;  //TODO: исправить дублирующийся центр. Либо в коллайдер, либо в физ объект
}

void TPhysObject::ApplyImpulse( Vector2f j, Vector2f r )
{
  //velocity += j / _mass;
  //w += r.PerpDot(j) / i;
};

//Изменяем скорость в соответствии с силой, которую присвоили после исправления коллизии
void TPhysObject::updateSpeed( float deltaTime )
{
  speed += speedChange;
  speedChange.setX(0); speedChange.setY(0);
}

TPhysObject::TPhysObject(Vector2f startPosition, Vector2f startSpeed)
{
  center = startPosition;
  speed = startSpeed;
//   m_collider = TCollider();
//   m_collider.m_center = center;
//   m_collider.m_colliderType = COLLIDER_CIRCLE;
}



