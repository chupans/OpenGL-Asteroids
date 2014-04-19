#include "TPhysEngine.h"

void TPhysEngine::physStep( float deltaTime )
{
  if (deltaTime > maxDeltaTime)
    return;

  //printf("DeltaTime is: %.8f \n", deltaTime);
  vector<TPhysObject>::iterator objIt, otherObjIt;
  for (objIt = m_existingObjects.begin(); objIt < m_existingObjects.end(); objIt++) //Лучше создать массив коллизий(т.к. таким образом не будут обрабатываться коллизии, где вошли три круга в друг друга). Дубликаты удалить
    objIt->wasProcessed = false;
  for (objIt = m_existingObjects.begin(); objIt < m_existingObjects.end(); objIt++)
  {
    objIt->updatePosition(deltaTime);
    objIt->updateSpeed(deltaTime);
    for (otherObjIt = m_existingObjects.begin(); otherObjIt < m_existingObjects.end(); otherObjIt++)
    {
      if (objIt != otherObjIt && !objIt->wasProcessed)
      {
        if (otherObjIt->m_collider.DoCollideWith(objIt->m_collider))
        {
          fixCollisions(*objIt, *otherObjIt);
          //process collission
          objIt->wasProcessed = true;
          otherObjIt->wasProcessed = true;
        }
      }
    }
  }
}

TPhysEngine::TPhysEngine()
{

}

void TPhysEngine::fixCollisions( TPhysObject &obj1, TPhysObject &obj2 )//перемещаем объекты, добавляем силы
{
  Vector2f v2 = obj2.speed;
  Vector2f v1 = obj1.speed;
  v2 *= obj2._mass/obj1._mass;
  obj1.speed = v2;

  v1*= obj1._mass/obj2._mass;
  obj2.speed = v1;

  while (obj1.m_collider.DoCollideWith(obj2.m_collider))
  {
    obj1.updatePosition(0.005f);
    obj2.updatePosition(0.005f);
  }

}
