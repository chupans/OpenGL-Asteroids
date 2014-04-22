#include "CCompositeBody.h"
#include "Functions.h"
#include "TConstants.h"

using namespace std;

CCompositeBody::CCompositeBody(void)
{

}

CCompositeBody::~CCompositeBody(void)
{
  delete m_collider;
}

void CCompositeBody::Render()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glDrawElements(GL_LINE_STRIP, m_collider->_edges.size() + 1, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

Vector2f CCompositeBody::GetPosition()
{
  return m_collider->GetPosition();
}

void CCompositeBody::SetAngularSpeedChange( float angSpeedChange )
{
  m_angularSpeedChange = angSpeedChange;
}

void CCompositeBody::RenderDebug( GLint loc )
{
  vector<TEdge>::iterator it;
  vector<Vector2f> points;
  for (it = m_collider->_edges.begin(); it < m_collider->_edges.end(); it++)
  {
    points.push_back(it->_p1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_debugVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(points.size()), &points[0], GL_DYNAMIC_DRAW);

  glUniform1i(loc, (GLint)1);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glDrawElements(GL_LINE_STRIP, points.size()+1, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glUniform1i(loc, (GLint)0);
}

void CCompositeBody::UpdatePositionAndAngle( float deltaTime )
{
  Vector2f shift = m_speed * deltaTime;

  if (m_isStatic)
    return;

  m_collider->UpdatePosition(shift);

  m_angle = Clamp<float>(m_angle, 0, 2*M_PI);
  m_angularSpeed = Clamp<float>(m_angularSpeed, -TConstants::maxAngularSpeed, TConstants::maxAngularSpeed);

  m_angle += m_angularSpeed*deltaTime;
  m_collider->Rotate(m_angle, m_angularSpeed*deltaTime);
  m_rotMatrix.initRotateTransform(m_angle);
}

bool CCompositeBody::IsStatic()
{
  return m_isStatic;
}

Vector2f CCompositeBody::GetSpeed()
{
  return m_speed;
}

Matrix2f CCompositeBody::GetRotMatrix()
{
  return m_rotMatrix;
}

void CCompositeBody::UpdateSpeed()
{
  if (m_isStatic)
    return;
  m_speed += m_speedChange;
  m_speedChange = Vector2f::ZERO;
  m_angularSpeed += m_angularSpeedChange;
  m_angularSpeedChange = 0;
}

ICollider* CCompositeBody::GetCollider()
{
  return (ICollider*) m_collider;
}

float CCompositeBody::GetAngularSpeed()
{
  return m_angularSpeed;
}

void CCompositeBody::SetAngle( float angle )
{
  m_angle = angle;
}

void CCompositeBody::SetSpeedChange( Vector2f speedChange )
{
  m_speedChange = speedChange;
}

void CCompositeBody::MultiplySpeedBy( float factor )
{
  m_speed *= factor;
}

void CCompositeBody::UpdatePosition( float deltaTime )
{
  Vector2f shift = m_speed * deltaTime;
  if (m_isStatic)
    return;
  m_collider->UpdatePosition(shift);
}

void CCompositeBody::AddToAngularSpeed( float angSpeed )
{
  m_angularSpeed += angSpeed;
}

float CCompositeBody::GetMass()
{
  return m_mass;
}

float CCompositeBody::GetAngle()
{
  return m_angle;
}
