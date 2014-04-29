#include "TConstants.h"
#include "Functions.h"
#include "CPolyBody.h"

using namespace std;

void CPolyBody::Render()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glDrawElements(GL_LINE_STRIP, m_collider->_edges.size() + 1, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

//Render without using rotation matrix. Renders actual point position.
void CPolyBody::RenderDebug (GLint loc)
{
  vector<TEdge>::iterator it;
  vector<Vector2f> points;
  for (it = m_collider->_edges.begin(); it != m_collider->_edges.end(); it++)
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

void CPolyBody::UpdatePositionAndAngle (float deltaTime)
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

Vector2f CPolyBody::GetSpeed()
{
  return m_speed;
}

void CPolyBody::UpdateSpeed()
{
  if (m_isStatic)
  {
    m_collider->UpdatePosition(m_speedChange);
    return;
  }
  m_speed += m_speedChange;
  m_speedChange = Vector2f::ZERO;
  m_angularSpeed += m_angularSpeedChange;
  m_angularSpeedChange = 0;
}

void CPolyBody::UpdatePosition( float deltaTime )
{
  Vector2f shift = m_speed * deltaTime;
  if (m_isStatic)
    return;
  m_collider->UpdatePosition(shift);
}

float CPolyBody::GetMass()
{
  return m_mass;
}

// float CPolyBody::GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse )
// {
//   Vector2f armVector = m_collider->m_center - armPoint;
//   return armVector.GetLength();
// }

ICollider* CPolyBody::GetCollider()
{
  return (ICollider*) m_collider;
}

CPolyBody::CPolyBody( Vector2f points[], unsigned int count, Vector2f position, Vector2f startSpeed, bool isStatic, float angularSpeed )
{
  unsigned int i;
  vector<unsigned int> indices;
  m_rotMatrix.InitIdentity();
  m_speed = startSpeed;
  m_speedChange = Vector2f::ZERO;
  m_isStatic = isStatic;
  m_angle = 0;
  m_angularSpeed = angularSpeed;
  m_collider = new CPolyCollider(points, count, position);
  m_angularSpeedChange = 0;
  for (i = 0; i < count; i++)
      indices.push_back(i);
  indices.push_back(0);

  m_mass = fabs(PolyFunctions::CalcPolyArea(points, count));

  glGenBuffers(1, &(m_vbo));
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(count), points, GL_STATIC_DRAW);

  glGenBuffers(1, &(m_ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &(m_debugVbo));
  glBindBuffer(GL_ARRAY_BUFFER, m_debugVbo);
}

Vector2f CPolyBody::GetPosition()
{
  return m_collider->GetPosition();
}

CPolyBody::~CPolyBody()
{
  delete m_collider;
}

bool CPolyBody::IsStatic()
{
  return m_isStatic;
}

void CPolyBody::SetSpeedChange( Vector2f speedChange )
{
  m_speedChange = speedChange;
}

void CPolyBody::SetAngle( float angle )
{
  m_angle = angle;
}

void CPolyBody::MultiplySpeedBy( float factor )
{
  m_speed *= factor;
}

void CPolyBody::AddToAngularSpeed( float angSpeed )
{
  m_angularSpeed += angSpeed;
}

float CPolyBody::GetAngle()
{
  return m_angle;
}

Matrix2f CPolyBody::GetRotMatrix()
{
  return m_rotMatrix;
}

float CPolyBody::GetAngularSpeed()
{
  return m_angularSpeed;
}

void CPolyBody::SetAngularSpeedChange( float angSpeedChange )
{
  m_angularSpeedChange = angSpeedChange;
}
