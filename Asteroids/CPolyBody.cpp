#include "TConstants.h"
#include "CPolyBody.h"
#include "Functions.h"

void CPolyBody::Render()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glDrawElements(GL_LINE_STRIP, m_collider->_edges.size() + 1, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

//Render without using rotation matrix. Renders actual point position.
void CPolyBody::RenderDebug (GLint loc)
{
  vector<TEdge>::iterator it;
  vector<Vector2f> points;
  for (it = m_collider->_edges.begin(); it < m_collider->_edges.end(); it++)
  {
    points.push_back(it->_p1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, realVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(points.size()), &points[0], GL_DYNAMIC_DRAW);

  glUniform1i(loc, (GLint)1);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

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
    return;
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

float CPolyBody::GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse )
{
  Vector2f armVector = m_collider->m_center - armPoint;
  return armVector.GetLength();
}

ICollider* CPolyBody::GetCollider()
{
  return (ICollider*) m_collider;
}

CPolyBody::CPolyBody( Vector2f points[], unsigned int count, Vector2f center, Vector2f startSpeed, bool isStatic, float angularSpeed )
{
  float area = 0;
  unsigned int i;
  vector<unsigned int> indices;
  m_rotMatrix.InitIdentity();
  m_speed = startSpeed;
  m_speedChange = Vector2f::ZERO;
  m_isStatic = isStatic;
  m_angle = 0;
  m_angularSpeed = angularSpeed;
  m_collider = new TMeshCollider(points, count, center);
  m_angularSpeedChange = 0;
  indices.push_back(0);
  for (i = 0; i < count; i++)
  {
    if (i+1 < count)
    {
      indices.push_back(i+1);
      area += (points[i].getX() + points[i+1].getX()) * (points[i].getY() - points[i+1].getY());
    }
    else
    {
      indices.push_back(0);
      area += (points[i].getX() + points[0].getX()) * (points[i].getY() - points[0].getY());
    }
  }
  float test = Vector2f::distanceBetween(center, points[0]);
  test = Vector2f::distanceBetween(center, points[1]);
  test = Vector2f::distanceBetween(center, points[2]);
  test = Vector2f::distanceBetween(center, points[3]);

  m_mass = fabs(area);

  glGenBuffers(1, &(vbo));
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(count), points, GL_STATIC_DRAW);

  glGenBuffers(1, &(ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &(realVbo));
  glBindBuffer(GL_ARRAY_BUFFER, realVbo);
}

Vector2f CPolyBody::GetPosition()
{
  return m_collider->m_center;
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
