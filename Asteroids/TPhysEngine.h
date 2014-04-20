#include "TPhysObject.h"
#include <vector>
using namespace std;

class TPhysEngine
{
public:
  TPhysEngine();
  void physStep(float deltaTime);

private:
  void findCollisions();
  void fixCollisions(TPhysObject &obj1, TPhysObject &obj2, Vector2f collidePoint);

public:
  float maxDeltaTime;
  vector<TPhysObject> m_existingObjects;

};