#include "CCompositeBody.h"
#include "Functions.h"
#include "TConstants.h"

using namespace std;

CCompositeBody::CCompositeBody(Vector2f **points, int count[], int objCount, Vector2f position, Vector2f startSpeed, bool isStatic, float angularSpeed)
{
  int i;
  m_collider = new CCompositeCollider(points, count, objCount, position);
  m_rotMatrix.InitIdentity();
  m_speed = startSpeed;
  m_speedChange = Vector2f::ZERO;
  m_isStatic = isStatic;
  m_angle = 0;
  m_angularSpeed = angularSpeed;
  m_angularSpeedChange = 0;
  indicesCount = 0;
  vector<Vector2f> _points;
  vector<unsigned int> indices;
  int curCount = 0;

  for (m_mass = 0, i = 0; i < objCount; i++)
  {
    m_mass += fabs(PolyFunctions::CalcPolyArea(points[i], count[i]));
    for (int j = 0; j < count[i]; j++)
    {
      _points.push_back(points[i][j]);
      indices.push_back(curCount);
      curCount++;
      indicesCount++;
    }
    indices.push_back(curCount - count[i]);
    indicesCount++;
  }

  glGenBuffers(1, &(m_vbo));
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(indicesCount), &_points[0], GL_STATIC_DRAW);

  glGenBuffers(1, &(m_ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &(m_debugVbo));
  glBindBuffer(GL_ARRAY_BUFFER, m_debugVbo);
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

  glDrawElements(GL_LINE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);

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
  vector<CPolyCollider*>::iterator collIt;
  vector<Vector2f> points;
  for (collIt = m_collider->m_polyColliders.begin(); collIt != m_collider->m_polyColliders.end(); collIt++)
  {
    for (it = (*collIt)->_edges.begin(); it != (*collIt)->_edges.end(); it++)
    {
      points.push_back(it->_p1);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_debugVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(points.size()), &points[0], GL_DYNAMIC_DRAW);

  glUniform1i(loc, (GLint)1);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

  glDrawElements(GL_LINE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);

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
