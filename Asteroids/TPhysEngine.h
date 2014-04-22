#pragma once
#include "IBody.h"
#include "CCollision.h"
#include <vector>

class TPhysEngine
{
public:
  TPhysEngine();
  void physStep(float deltaTime);

private:
  void findCollisions();
  void FixCollisions(IBody *obj1, IBody *obj2, Vector2f collidePoint);

public:
  int nCollision;
  float maxDeltaTime;
  std::vector<IBody*> m_existingBodies;
  std::vector<CCollision> m_existingCollissions;

};