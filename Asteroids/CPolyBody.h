#pragma once
#include "IBody.h"
#include "TMeshCollider.h"

class CPolyBody : public IBody
{
private:
  Vector2f m_speed; // ускорение
  Vector2f m_speedChange;   //сила
  float m_angle;   // текущий угол в радианах
  float m_angularSpeed;   // угловая скорость в радианах
  float m_angularSpeedChange;
  float m_mass;   // масса
  bool m_isStatic;
  TMeshCollider *m_collider;
  bool wasProcessed;
  GLuint vbo, realVbo;
  GLuint ibo;
  Matrix2f m_rotMatrix;

public:
  CPolyBody(Vector2f points[], unsigned int count, Vector2f center, Vector2f startSpeed, bool isStatic, float angularSpeed);
  ~CPolyBody();
  virtual void Render();
  virtual void RenderDebug( GLint loc );
  virtual void UpdatePositionAndAngle( float deltaTime );
  virtual Vector2f GetSpeed();
  virtual void UpdateSpeed();
  virtual void UpdatePosition( float deltaTime );
  virtual float GetMass();
  virtual float GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse );
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

};