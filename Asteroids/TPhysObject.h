#include "math_3d.h"
#include "TCollider.h"

class TPhysObject
{
public:
  TPhysObject(Vector2f startPosition, Vector2f startSpeed);
  Vector2f center; // �������
  Vector2f speed; // ���������
  Vector2f speedChange;   //����
  float angle;   // ������� ���� � ��������
  float angularSpeed;   // ������� �������� � ��������
  float _mass;   // �����
  float _friction;   // ������
  bool isStatic;
  TMeshCollider m_collider;
  bool wasProcessed;

  // ������� ����������� ������� �� ����
  // j - ������� (������)
  // r - ����� ���������� �������� � ��������� �����������
  void ApplyImpulse(Vector2f j, Vector2f r);
  void updatePosition(float deltaTime);
  void updateSpeed(float deltaTime);
  float GetMomentumArmLength(Vector2f armPoint);
};