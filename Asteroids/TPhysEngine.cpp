#include "TPhysEngine.h"

void TPhysEngine::physStep( float deltaTime )
{
  if (deltaTime > maxDeltaTime)
    return;

  vector<IBody*>::iterator objIt, otherObjIt;
  Vector2f collidePoint = Vector2f::ZERO;
  for (objIt = m_existingBodies.begin(); objIt < m_existingBodies.end(); objIt++) //FIXME: Лучше создать массив коллизий(т.к. таким образом не будут обрабатываться коллизии, где вошли три круга в друг друга). Дубликаты удалить
    (*objIt)->wasProcessed = false;
  for (objIt = m_existingBodies.begin(); objIt < m_existingBodies.end(); objIt++)
  {
    (*objIt)->UpdateSpeed();
    (*objIt)->UpdatePositionAndAngle(deltaTime);
    if (!(*objIt)->IsStatic())
    {
      for (otherObjIt = m_existingBodies.begin(); otherObjIt < m_existingBodies.end(); otherObjIt++)
      {
        if ((*objIt) != (*otherObjIt) && !(*objIt)->wasProcessed)
        {
          if ((*objIt)->GetCollider()->DoCollideWith((*otherObjIt)->GetCollider(), collidePoint))//FIXME: Вызывать для обоих?
          {
            FixCollisions(*objIt, *otherObjIt, collidePoint);
            //process collission
            (*objIt)->wasProcessed = true;
            (*otherObjIt)->wasProcessed = true;
          }
        }
      }
    }
  }
}

TPhysEngine::TPhysEngine()
{
  nCollision = 0;
}

void TPhysEngine::FixCollisions( IBody *obj1, IBody *obj2, Vector2f collidePoint )
{
  Vector2f v2 = obj2->GetSpeed();
  Vector2f v1 = obj1->GetSpeed();
  Vector2f v1temp = v1;
  Vector2f v2temp = v2;
  Vector2f newV1 = Vector2f::ZERO;
  Vector2f newV2 = Vector2f::ZERO;

  float m1 = obj1->GetMass();
  float m2 = obj2->GetMass();
  nCollision++;
  if (obj2->IsStatic())
  {
    Vector2f reaction = obj2->GetCollider()->GetCollissionVector(obj1->GetCollider());
    newV1 = v1;
        // HACK: только бандуари pod прямым углом
    if (reaction.getY() == 0)
    {
      newV1.setX(-1 * obj1->GetSpeed().getX());
      obj1->SetSpeedChange(newV1 - v1);
    }
    else
    {
      newV1.setY(-1 * obj1->GetSpeed().getY());
      obj1->SetSpeedChange(newV1 - v1);
    }
    obj1->AddToAngularSpeed(-2*(obj1->GetAngularSpeed()));
  }
  else
  {
    float angleFactor, momentumArmLength, direction;
    Vector2f obj1Arm, obj2Arm;
    direction = Vector2f::zCrossProduct(obj2->GetPosition(), obj2->GetPosition() + v2, collidePoint);
    if (direction != 0)
    {
      direction = direction > 0 ? 1 : -1;
    obj1Arm = obj1->GetPosition() - collidePoint;
    momentumArmLength = obj1Arm.GetLength();
    obj1Arm.normalize();
    v2temp.normalize();
    angleFactor = 1.0f - fabs(obj1Arm.dotProduct(v2temp)); //max when angle between vectors is 90
    obj1->SetAngularSpeedChange(-40*momentumArmLength * (m2/m1) * angleFactor * direction);
    }

    direction = Vector2f::zCrossProduct(obj1->GetPosition(), obj1->GetPosition() + v1, collidePoint);
    if (direction != 0)
    {
      direction = direction > 0 ? 1 : -1;
    obj2Arm = obj2->GetPosition() - collidePoint;
    momentumArmLength = obj2Arm.GetLength();
    obj2Arm.normalize();
    v1temp.normalize();
    angleFactor = 1.0f - fabs(obj2Arm.dotProduct(v1temp)); //max when angle between vectors is 90
    obj2->SetAngularSpeedChange(-40*momentumArmLength * (m1/m2) * angleFactor * direction);
    }

    newV2 = ( m1 * v1 + m2 * v2 + (v1 - v2) * m1) / (m1 + m2);
    newV1 = newV2 - v1 + v2;

    obj1->SetSpeedChange(newV1 - v1);
    obj2->SetSpeedChange(newV2 - v2);
//     obj1.speed = v1/5 + (v2 * m2 * 4)/(m1 * 5);
//     obj2.speed = v2/5 + (v1 * m1 * 4)/(m2 * 5);
  }
  obj1->MultiplySpeedBy(-1.0f);
  obj2->MultiplySpeedBy(-1.0f);
  float ang1 = obj1->GetAngle();
  float ang2 = obj2->GetAngle();
  obj1->AddToAngularSpeed(obj1->GetAngularSpeed() * (-2));
  obj2->AddToAngularSpeed(obj2->GetAngularSpeed() * (-2));

  //Вызывать для обоих?
  while (obj1->GetCollider()->DoCollideWith(obj2->GetCollider()))  //HACK: moving our objects a little till they don't collide
  {
    obj1->UpdatePositionAndAngle(0.001f);
    obj2->UpdatePositionAndAngle(0.001f);
  }
  obj1->GetCollider()->Rotate(ang1, -obj1->GetAngle() + ang1);
  if (!obj2->IsStatic())
    obj2->GetCollider()->Rotate(ang2, -obj2->GetAngle() + ang2); //FIXME
  obj1->SetAngle(ang1);
  obj2->SetAngle(ang2);
  obj1->MultiplySpeedBy(-1.0f);
  obj2->MultiplySpeedBy(-1.0f);
  obj1->AddToAngularSpeed(obj1->GetAngularSpeed() * (-2));
  obj2->AddToAngularSpeed(obj2->GetAngularSpeed() * (-2));
}
