#include "math_3d.h"
#include "TCollider.h"

class TPhysObject
{
public:
  TPhysObject(Vector2f startPosition, Vector2f startSpeed);
  Vector2f center; // позиция
  Vector2f speed; // ускорение
  Vector2f speedChange;   //сила
  float angle;   // текущий угол в радианах
  float angularSpeed;   // угловая скорость в радианах
  float _mass;   // масса
  float _friction;   // трение
  bool isStatic;
  TMeshCollider m_collider;
  bool wasProcessed;

  // функция накладывает импульс на тело
  // j - импульс (вектор)
  // r - точка приложения импульса в локальных координатах
  void ApplyImpulse(Vector2f j, Vector2f r);
  void updatePosition(float deltaTime);
  void updateSpeed(float deltaTime);
  float GetMomentumArmLength(Vector2f armPoint);
};