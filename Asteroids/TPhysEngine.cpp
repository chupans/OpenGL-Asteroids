#include "TPhysEngine.h"
#include <algorithm>

using namespace std;

bool shouldDelete (const CCollision& value) { return (value.m_done); }

void TPhysEngine::physStep( float deltaTime )
{
  if (deltaTime > maxDeltaTime)
    return;

  vector<IBody*>::iterator objIt, otherObjIt;
  list<CCollision>::iterator collIt;
  Vector2f collidePoint = Vector2f::ZERO;
  for (objIt = m_existingBodies.begin(); objIt != m_existingBodies.end(); objIt++)
  {
    if (!(*objIt)->IsStatic())
    {
      for (otherObjIt = m_existingBodies.begin(); otherObjIt != m_existingBodies.end(); otherObjIt++)
      {
        if ((*objIt) != (*otherObjIt))
        {
          CCollision newColl(*objIt, *otherObjIt);
          collIt = find(m_existingCollissions.begin(), m_existingCollissions.end(), newColl);
          if (collIt == m_existingCollissions.end() /*&& (*objIt)->GetCollider()->ProbablyCollideWith((*otherObjIt)->GetCollider())*/)
            m_existingCollissions.push_back(newColl);
        }
      }
    }
  }
  //m_existingCollissions.unique();

  for (collIt = m_existingCollissions.begin(); collIt != m_existingCollissions.end(); collIt++)
  {
    collIt->ProcessCollision(deltaTime);
  }

  m_existingCollissions.remove_if(shouldDelete);

  for (objIt = m_existingBodies.begin(); objIt != m_existingBodies.end(); objIt++)
  {
    (*objIt)->UpdateSpeed();
    (*objIt)->UpdatePositionAndAngle(deltaTime);
  }
}

TPhysEngine::TPhysEngine()
{
  nCollision = 0;
}

void TPhysEngine::FixCollisions( IBody *obj1, IBody *obj2, Vector2f collidePoint )
{
  
}
