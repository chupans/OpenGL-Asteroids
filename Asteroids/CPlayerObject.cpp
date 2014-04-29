#include "CPlayerObject.h"

CPlayerObject::CPlayerObject(CPolyBody *body)
{
  m_body = body;
}

CPlayerObject::~CPlayerObject(void)
{
}

void CPlayerObject::Move( MovementDir dir )
{
  Vector2f movement = Vector2f::ZERO;
  switch (dir)
  {
  case UP:
    movement = Vector2f(0.0f, 1.0f);
  	break;
  case RIGHT:
    movement = Vector2f(1.0f, 0.0f);
    break;
  case DOWN:
    movement = Vector2f(0.0f, -1.0f);
    break;
  case LEFT:
    movement = Vector2f(-1.0f, 0.0f);
    break;
  }
  movement /= 10;
  m_body->SetSpeedChange(movement);
  m_body->UpdateSpeed();
}

IBody* CPlayerObject::GetBody()
{
  return m_body;
}

bool CPlayerObject::IsAlive()
{
  throw std::exception("The method or operation is not implemented.");
}
