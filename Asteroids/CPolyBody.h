#pragma once
#include "IBody.h"
#include "CPolyCollider.h"

class CPolyBody : public IBody
{
private:
  Vector2f m_speed;
  Vector2f m_speedChange;         //изменение скорости на следующем шаге
  float m_angle;                  // текущий угол в радианах
  float m_angularSpeed;           // изменение угловой скорости на следующем шаге
  float m_angularSpeedChange;     // изменение изменения угловой скорости на следующем шаге (Lol) FIXME
  float m_mass;
  bool m_isStatic;
  CPolyCollider *m_collider;
  bool m_delete;
  GLuint m_vbo, m_debugVbo;
  GLuint m_ibo;
  Matrix2f m_rotMatrix;

public:
  CPolyBody(Vector2f points[], unsigned int count, Vector2f position, Vector2f startSpeed, bool isStatic, float angularSpeed);
  ~CPolyBody();
  virtual void Render();
  virtual void RenderDebug( GLint loc );
  virtual void UpdatePositionAndAngle( float deltaTime );
  virtual Vector2f GetSpeed();
  virtual void UpdateSpeed();
  virtual void UpdatePosition( float deltaTime );
  virtual float GetMass();
  //virtual float GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse );
  virtual ICollider* GetCollider();
  virtual Vector2f GetPosition();
  virtual bool IsStatic();
  virtual void SetSpeedChange( Vector2f speedChange );
  virtual void SetAngle( float angle );
  virtual void MultiplySpeedBy( float factor );
  virtual void AddToAngularSpeed( float angSpeed );
  virtual float GetAngle();
  virtual Matrix2f GetRotMatrix();
  virtual float GetAngularSpeed();

  virtual void SetAngularSpeedChange( float angSpeedChange );

  virtual bool MarkedToDelete();

  virtual void Delete();

};