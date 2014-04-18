#include "TPhysEngine.h"

void TPhysEngine::physStep( float deltaTime )
{
  if (deltaTime > maxDeltaTime)
    return;

  printf("DeltaTime is: %.8f \n", deltaTime);
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
        if (otherObjIt->m_collider.doCollideWith(objIt->m_collider))
        {
          fixCollisions(*objIt, *otherObjIt);
          //process collission
          otherObjIt->wasProcessed = true;
          objIt->wasProcessed = true;
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
  float forceMagnitude = obj1.m_collider.getCollisionLength(obj2.m_collider);
  Vector2f force(obj1.center.getX() - obj2.center.getX(), obj1.center.getY() - obj2.center.getY());
  force.normalize();



  obj1.force = force;



}
