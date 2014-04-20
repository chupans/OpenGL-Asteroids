#include "TCollider.h"

bool TCollider::isPointInside( Vector2f point )
{
  switch (m_colliderType)
  {
  case COLLIDER_CIRCLE:
    float distance;
    distance = Vector2f::squareDistanceBetween(m_center, point);
    if (distance < m_radius * m_radius)
      return true;
    else
      return false;
    break;
  }
}

bool TCollider::doCollideWith( TCollider other )
{
  switch (m_colliderType)
  {
  case COLLIDER_CIRCLE:
    float distance;
    distance = Vector2f::squareDistanceBetween(this->m_center, other.m_center);
    distance -= this->m_radius * this->m_radius + other.m_radius * other.m_radius;
    if (distance > 0)
      return false;
    else
      return true;
    break;
  }
}

float TCollider::getCollisionLength( TCollider other )
{
  float distance;
  distance = Vector2f::squareDistanceBetween(this->m_center, other.m_center);
  distance -= this->m_radius * this->m_radius + other.m_radius * other.m_radius;
  return fabs(distance);
}

TEdge::TEdge( Vector2f startPoint, Vector2f endPoint ) 
{
  _p1 = startPoint;
  _p2 = endPoint;
  Vector2f edgenormal( _p2.getY() - _p1.getY(), - _p2.getX() + _p1.getX()); // edge vector is (a,b), normal vector is (b, -a)
  _normal = edgenormal;
  _normal.normalize();

}

Vector2f TEdge::getEdgeVector()
{
  Vector2f ret(_p2.getX() - _p1.getX(), _p2.getY() - _p1.getY());
  return ret;
}

// < 0 слева. z координата векторного произведения
float TEdge::onWhichSide( Vector2f _point )
{
  Vector2f a = this->getEdgeVector();
  Vector2f b(_point.getX() - _p2.getX(), _point.getY() - _p2.getY());

  return (a.getX() * b.getY() - a.getY() * b.getX());
}

//Генерирует точки сама
TMeshCollider::TMeshCollider()
{

}

//Обход точек против часовой стрелки, создание соответствующих граней. Инициализация буффера вершин и индексов.
TMeshCollider::TMeshCollider( Vector2f points[], unsigned int count, Vector2f center )
{
  unsigned int i;
  this->vbo = 0;
  rotMatrix.InitIdentity();
  vector<unsigned int> indices;
  indices.push_back(0);
  for (i = 0; i < count; i++)
  {
    if (i+1 < count)
    {
    _edges.push_back(TEdge(points[i], points[i+1]));
    indices.push_back(i+1);
    }
  }
  _edges.push_back(TEdge(points[count-1], points[0]));
  indices.push_back(0);
  this->m_center = center;

  glGenBuffers(1, &(this->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*(count), points, GL_STATIC_DRAW);

  glGenBuffers(1, &(this->ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
}

bool TMeshCollider::IsPointInside( Vector2f point )
{
  vector<TEdge>::iterator edgesIt;

  for (edgesIt = _edges.begin(); edgesIt < _edges.end(); edgesIt++)
  {
    if (edgesIt->onWhichSide(point) <= 0)
      return false;
  }
  return true;
}

bool TMeshCollider::DoCollideWith(TMeshCollider other)
{
  Vector2f temp = Vector2f::ZERO;
  return DoCollideWith(other, temp);
}

bool TMeshCollider::DoCollideWith( TMeshCollider other, Vector2f &collidePoint )
{
  vector<TEdge>::iterator edgesIt;
  vector<Vector2f> points;
  vector<Vector2f>::iterator pIt;
  bool doTheyCollide = false;

  for (edgesIt = this->_edges.begin(); edgesIt < this->_edges.end(); edgesIt++)
  {
    if (other.IsPointInside(edgesIt->_p1))
    {
      points.push_back(edgesIt->_p1);
      doTheyCollide = true;
    }
  }
  for (edgesIt = other._edges.begin(); edgesIt < other._edges.end(); edgesIt++)
  {
    if (this->IsPointInside(edgesIt->_p1))
    {
      points.push_back(edgesIt->_p1);
      doTheyCollide = true;
    }
  }

  collidePoint = Vector2f::ZERO;
  for (pIt = points.begin(); pIt < points.end(); pIt++)
  {
    collidePoint += *pIt/points.size();
  }
  return doTheyCollide;
}

void TMeshCollider::Render()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);

  glDrawElements(GL_LINE_STRIP, _edges.size() + 1, GL_UNSIGNED_INT, 0);
  //glDrawRangeElements(GL_TRIANGLES, start, end, 6, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

void TMeshCollider::UpdatePosition( Vector2f shift )
{
  vector<TEdge>::iterator edgesIt;
  for (edgesIt = _edges.begin(); edgesIt < _edges.end(); edgesIt++)
  {
    edgesIt->_p1 += shift;
    edgesIt->_p2 += shift;
  }
  this->m_center += shift;
}

void TMeshCollider::Rotate( float angle )
{
  vector<TEdge>::iterator edgeIt;

  for (edgeIt = _edges.begin(); edgeIt < _edges.end(); edgeIt++)
  {
    edgeIt->_p1.Rotate(m_center, angle);
    edgeIt->_p2.Rotate(m_center, angle);
  }
  rotMatrix.initRotateTransform(angle);
}
