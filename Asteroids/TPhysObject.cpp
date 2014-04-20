#include "TPhysObject.h"

void TPhysObject::updatePosition( float deltaTime )
{
  Vector2f shift = speed * deltaTime;
  if (this->isStatic)
    return;
  this->center += shift;
  this->m_collider.UpdatePosition(shift);
  this->m_collider.m_center = this->center;  //TODO: ��������� ������������� �����. ���� � ���������, ���� � ��� ������
  this->angle += angularSpeed*deltaTime;
  this->m_collider.Rotate(this->angle);
}

void TPhysObject::ApplyImpulse( Vector2f j, Vector2f r )
{
  //velocity += j / _mass;
  //w += r.PerpDot(j) / i;
};

//�������� �������� � ������������ � �����, ������� ��������� ����� ����������� ��������
void TPhysObject::updateSpeed( float deltaTime )
{
  if (this->isStatic)
    return;
  speed += speedChange;
  speedChange.setX(0); speedChange.setY(0);
}

TPhysObject::TPhysObject(Vector2f startPosition, Vector2f startSpeed)
{
  center = startPosition;
  speed = startSpeed;
  isStatic = false;
  this->_mass = 0.0f;
//   m_collider = TCollider();
//   m_collider.m_center = center;
//   m_collider.m_colliderType = COLLIDER_CIRCLE;
}

float TPhysObject::GetMomentumArmLength( Vector2f armPoint )
{
  // ����������� ��������� ������ ������������ �������� 1 �������. Ax + By + C = 0
  float A = -this->speed.getX();
  float B = -this->speed.getY();
  float C = -A * this->center.getX() - B * this->center.getY();

  //���������� �� ����� M �� ������ Ax + By + C = 0. A*Mx + B*My + C /sqrt(A^2 + B^2)
  return (A * armPoint.getX() + B * armPoint.getY() + C) / sqrtf(A * A + B * B);
}



