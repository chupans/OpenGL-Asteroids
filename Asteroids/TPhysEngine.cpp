#include "TPhysEngine.h"

void TPhysEngine::physStep( float deltaTime )
{
  if (deltaTime > maxDeltaTime)
    return;

  //printf("DeltaTime is: %.8f \n", deltaTime);
  vector<TPhysObject>::iterator objIt, otherObjIt;
  Vector2f collidePoint = Vector2f::ZERO;
  for (objIt = m_existingObjects.begin(); objIt < m_existingObjects.end(); objIt++) //Лучше создать массив коллизий(т.к. таким образом не будут обрабатываться коллизии, где вошли три круга в друг друга). Дубликаты удалить
    objIt->wasProcessed = false;
  for (objIt = m_existingObjects.begin(); objIt < m_existingObjects.end(); objIt++)
  {
    objIt->updatePosition(deltaTime);
    objIt->updateSpeed(deltaTime);
    if (!objIt->isStatic)
    {
      for (otherObjIt = m_existingObjects.begin(); otherObjIt < m_existingObjects.end(); otherObjIt++)
      {
        if (objIt != otherObjIt && !objIt->wasProcessed)
        {
          if (objIt->m_collider.DoCollideWith(otherObjIt->m_collider, collidePoint))
          {
            fixCollisions(*objIt, *otherObjIt, collidePoint);
            //process collission
            objIt->wasProcessed = true;
            otherObjIt->wasProcessed = true;
          }
        }
      }
    }
  }
}

TPhysEngine::TPhysEngine()
{

}

void TPhysEngine::fixCollisions( TPhysObject &obj1, TPhysObject &obj2, Vector2f collidePoint )
{
  Vector2f v2 = obj2.speed;
  Vector2f v1 = obj1.speed;

  float m1 = obj1._mass;
  float m2 = obj2._mass;
  if (obj2.isStatic)
  {
    // HACK:
    if (obj2.angle == 90)
    {
      obj1.speed.setX(-1 * obj1.speed.getX());
    }
    else
    {
      obj1.speed.setY(-1 * obj1.speed.getY());
    }
  }
  else
  {
    float momentumArmLength;
    momentumArmLength = obj1.GetMomentumArmLength(collidePoint);
    float angleFactor = 1 - fabs(v1.GetNormalized().dotProduct(v2.GetNormalized())); //max when angle between vectors is 90
    obj1.angularSpeed += 40*momentumArmLength*angleFactor;

    momentumArmLength = obj2.GetMomentumArmLength(collidePoint);
    obj2.angularSpeed += 40*momentumArmLength*angleFactor;

    obj2.speed = ( m1 * v1 + m2 * v2 + (v1 - v2) * m1) / (m1 + m2);
    obj1.speed = obj2.speed - v1 + v2;

//     obj1.speed = v1/5 + (v2 * m2 * 4)/(m1 * 5);
//     obj2.speed = v2/5 + (v1 * m1 * 4)/(m2 * 5);
  }

  while (obj1.m_collider.DoCollideWith(obj2.m_collider))  //HACK: moving our objects a little till they don't collide
  {
    obj1.updatePosition(0.005f);
    obj2.updatePosition(0.005f);
  }
}
