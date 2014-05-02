#pragma once
#include "IBody.h"
#include "CBoundaryCollider.h"
#include "math_3d.h"

class CBoundaryBody : IBody
{
public:
  CBoundaryBody (Vector2f startPoint, Vector2f endPoint, Vector2f pointInsideBody);
  ~CBoundaryBody(void);

  virtual void Render();
  virtual Vector2f GetPosition();
  virtual void RenderDebug( GLint loc );
  virtual void UpdatePositionAndAngle( float deltaTime );
  virtual Vector2f GetSpeed();
  virtual void UpdateSpeed();
  virtual ICollider* GetCollider();
  virtual void UpdatePosition( float deltaTime );
  virtual float GetMass();
  virtual float GetMomentumArmLength( Vector2f armPoint, Vector2f incImpulse );
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

private:
  CBoundaryCollider *m_collider;
  GLuint m_vbo, m_ibo;
  int m_pointsCount;
};
