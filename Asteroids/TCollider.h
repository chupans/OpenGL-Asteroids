#pragma once
#include "math_3d.h"
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

enum COLLIDER_TYPE
{
  COLLIDER_CIRCLE,
  COLLIDER_MESH
};

class TCollider
{
public:
  virtual bool isPointInside( Vector2f point );
  virtual bool doCollideWith( TCollider other );
  float getCollisionLength( TCollider other );

public:
  COLLIDER_TYPE m_colliderType;
  float m_radius;
  Vector2f m_center;
};

class TEdge
{
public:
  TEdge (Vector2f startPoint, Vector2f endPoint);
  Vector2f getEdgeVector();
  Vector2f _normal;
  Vector2f _p1, _p2;
  float onWhichSide(Vector2f _point);
};

class TMeshCollider
{
public:
  TMeshCollider();
  TMeshCollider(Vector2f points[], unsigned int count, Vector2f center);
  bool IsPointInside( Vector2f point);
  bool DoCollideWith( TMeshCollider other, Vector2f &collidePoint);
  bool DoCollideWith(TMeshCollider other);
  Vector2f GetCollissionVector(TMeshCollider other);
  void UpdatePosition(Vector2f shift);
  void Render();
  void Rotate( float angularSpeed );
  vector<TEdge> _edges;
  Vector2f m_center;
  GLuint vbo;
  GLuint ibo;
  Matrix2f rotMatrix;
  
};
