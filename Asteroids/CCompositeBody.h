#pragma once
#include "IBody.h"
#include "CCompositeCollider.h"

class CCompositeBody : public IBody
{
private:
  Vector2f m_speed;
  Vector2f m_speedChange;         //��������� �������� �� ��������� ����
  float m_angle;                  // ������� ���� � ��������
  float m_angularSpeed;           // ��������� ������� �������� �� ��������� ����
  float m_angularSpeedChange;     // ��������� ��������� ������� �������� �� ��������� ���� (Lol) FIXME
  float m_mass;
  bool m_isStatic;
  CPolyCollider *m_collider;
  bool wasProcessed;
  GLuint m_vbo, m_debugVbo;
  GLuint m_ibo;
  Matrix2f m_rotMatrix;

public:
  CCompositeBody(void);
  ~CCompositeBody(void);

  virtual void Render();
  virtual Vector2f GetPosition();
  virtual void SetAngularSpeedChange( float angSpeedChange );
  virtual void RenderDebug( GLint loc );
  virtual void UpdatePositionAndAngle( float deltaTime );
  virtual bool IsStatic();
  virtual Vector2f GetSpeed();
  virtual Matrix2f GetRotMatrix();
  virtual void UpdateSpeed();
  virtual ICollider* GetCollider();
  virtual float GetAngularSpeed();
  virtual void SetAngle( float angle );
  virtual void SetSpeedChange( Vector2f speedChange );
  virtual void MultiplySpeedBy( float factor );
  virtual void UpdatePosition( float deltaTime );
  virtual void AddToAngularSpeed( float angSpeed );
  virtual float GetMass();
  virtual float GetAngle();

};
