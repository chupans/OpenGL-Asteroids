#pragma once
#include "IGameObject.h"
#include "CPolyBody.h"

enum MovementDir
{
  LEFT,
  UP,
  RIGHT,
  DOWN
};

class CPlayerObject : public IGameObject
{
public:
  CPlayerObject(CPolyBody *body);
  ~CPlayerObject(void);
  void Move(MovementDir dir);
  void Shoot();
  IBody* GetBody();

  virtual bool IsAlive();

  int m_health;
  CPolyBody *m_body;

};
