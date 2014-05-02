#pragma once
#include "math_3d.h"
#include "ICollider.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

class IBody
{
public:
  virtual Vector2f GetPosition() = 0;
  virtual void UpdatePositionAndAngle(float deltaTime) = 0;
  virtual void UpdatePosition(float deltaTime) = 0;
  virtual void UpdateSpeed() = 0;
  virtual void Render() = 0;
  virtual void RenderDebug(GLint loc) = 0;
  virtual float GetMass() = 0;
  virtual Vector2f GetSpeed() = 0;
  virtual bool IsStatic() = 0;
  virtual ICollider* GetCollider() = 0;
  virtual void SetSpeedChange (Vector2f speedChange) = 0;
  virtual void SetAngularSpeedChange (float angSpeedChange) = 0;
  virtual void AddToAngularSpeed (float angSpeed) = 0;
  virtual float GetAngle() = 0;
  virtual void MultiplySpeedBy(float factor) = 0;
  virtual void SetAngle(float angle) = 0;
  virtual Matrix2f GetRotMatrix() = 0;
  virtual float GetAngularSpeed() = 0;
  virtual bool MarkedToDelete() = 0;
  virtual void Delete() = 0;

};