#pragma once
#include "IBody.h"

class CCollision
{
public:
  CCollision(IBody *body1, IBody *body2);
  bool operator ==(CCollision const &other);

  bool ProcessCollision(float deltaTime);

  bool m_done;
private:
  int stepsLeftIdle;    //Оставшееся кол-во шагов движка, в которые не обрабатывается данная коллизия
  IBody *m_body1;
  IBody *m_body2;
  float startDistance;
};
