#include "CBoundaryBody.h"

CBoundaryBody::CBoundaryBody(Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideBody)
{
  Vector2f points[2];
  int indices[2] = {0, 1};
  points[0] = startPoint;
  points[1] = endPoint;
  m_pointsCount = 2;

  m_collider = new CBoundaryCollider(startPoint, endPoint, pointInsideBody);

  glGenBuffers(1, &(m_vbo));
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(2), points, GL_STATIC_DRAW);

  glGenBuffers(1, &(m_ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

CBoundaryBody::~CBoundaryBody(void)
{
  delete m_collider;
}

void CBoundaryBody::Render()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glDrawElements(GL_LINE_STRIP, m_pointsCount, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

Vector2f CBoundaryBody::GetPosition()
{
  return Vector2f::ZERO;
}

void CBoundaryBody::RenderDebug( GLint loc )
{
  return;
}

void CBoundaryBody::UpdatePositionAndAngle( float deltaTime )
{
  return;
}

Vector2f CBoundaryBody::GetSpeed()
{
  return Vector2f::ZERO;
}

void CBoundaryBody::UpdateSpeed()
{
  return;
}

bool CBoundaryBody::IsStatic()
{
  return true;
}

ICollider* CBoundaryBody::GetCollider()
{
  return (ICollider*)m_collider;
}

void CBoundaryBody::UpdatePosition( float deltaTime )
{
  return;
}

float CBoundaryBody::GetMass()
{
  return 0;
}

float CBoundaryBody::GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse )
{
  return 0;
}

void CBoundaryBody::SetSpeedChange( Vector2f speedChange )
{
  return;
}

void CBoundaryBody::SetAngle( float angle )
{
  return;
}

void CBoundaryBody::MultiplySpeedBy( float factor )
{
  return;
}

void CBoundaryBody::AddToAngularSpeed( float angSpeed )
{
  return;
}

float CBoundaryBody::GetAngle()
{
  return 0;
}

Matrix2f CBoundaryBody::GetRotMatrix()
{
  Matrix2f temp;
  temp.InitIdentity();
  return temp;
}

float CBoundaryBody::GetAngularSpeed()
{
  return 0;
}

void CBoundaryBody::SetAngularSpeedChange( float angSpeedChange )
{
}

bool CBoundaryBody::MarkedToDelete()
{
  return false;
}

void CBoundaryBody::Delete()
{
  return;
}
