#pragma once
#include "tphysobject.h"

class TCircle :
  public TPhysObject
{
public:
  TCircle(void);
  ~TCircle(void);

private:
  float m_radius;

};
