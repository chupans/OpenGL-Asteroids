#include "math_3d.h"
#include "TCollider.h"

class TPhysObject
{
public:
  TPhysObject(Vector2f startPosition, Vector2f startSpeed);
  Vector2f center; // �������
  Vector2f speed; // ���������
  Vector2f force;   //����
  float angle;   // ������� ���� � ��������
  float angularVelocity;   // ������� ��������� � ��������
  float _mass;   // �����
  float _friction;   // ������
  bool isStatic;
  TCollider m_collider;
  bool wasProcessed;

  // ������� ����������� ������� �� ����
  // j - ������� (������)
  // r - ����� ���������� �������� � ��������� �����������
  void ApplyImpulse(Vector2f j, Vector2f r);
  void updatePosition(float deltaTime);
  void updateSpeed(float deltaTime);
};