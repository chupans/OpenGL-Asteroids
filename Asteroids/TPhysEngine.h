#include "IBody.h"
#include <vector>
using namespace std;

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
  vector<IBody*> m_existingBodies;

};