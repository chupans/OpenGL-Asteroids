#include "TMeshCollider.h"



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

void TEdge::Rotate( Vector2f pivotPoint, float angle )
{
  this->_p1.Rotate(pivotPoint, angle);
  this->_p2.Rotate(pivotPoint, angle);
}

void TEdge::UpdatePosition( Vector2f shift )
{
  this->_p1 += shift;
  this->_p2 += shift;
}



//Обход точек против часовой стрелки, создание соответствующих граней. Инициализация буффера вершин и индексов.
TMeshCollider::TMeshCollider( Vector2f points[], unsigned int count, Vector2f center )
{
  unsigned int i;
  for (i = 0; i < count; i++)
  {
    if (i+1 < count)
      _edges.push_back(TEdge(points[i] + center, points[i+1] + center));
  }
  _edges.push_back(TEdge(points[count-1] + center, points[0] + center));
  this->m_center = center;
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

bool TMeshCollider::DoCollideWith( ICollider *other, Vector2f &collidePoint )
{
  vector<TEdge>::iterator edgesIt;
  vector<Vector2f> points;
  vector<Vector2f>::iterator pIt;
  bool doTheyCollide = false;

  for (edgesIt = this->_edges.begin(); edgesIt < this->_edges.end(); edgesIt++)
  {
    if (other->IsPointInside(edgesIt->_p1))
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

bool TMeshCollider::DoCollideWith( ICollider *other )
{
  Vector2f temp = Vector2f :: ZERO;
  return this->DoCollideWith(other, temp);
}

void TMeshCollider::UpdatePosition( Vector2f shift )
{
  vector<TEdge>::iterator edgesIt;
  for (edgesIt = _edges.begin(); edgesIt < _edges.end(); edgesIt++)
  {
    edgesIt->UpdatePosition(shift);
  }
  this->m_center += shift;
}

void TMeshCollider::Rotate( float angle, float delta )
{
  vector<TEdge>::iterator edgeIt;

  for (edgeIt = _edges.begin(); edgeIt < _edges.end(); edgeIt++)
  {
    edgeIt->Rotate(m_center, delta);
  }
}

Vector2f TMeshCollider::GetCollissionVector( ICollider *other )
{
  //FIXME:
  return Vector2f::ZERO;
}
