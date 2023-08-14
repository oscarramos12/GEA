#pragma once
#include "ECS/System.h"

class UpdateSystem : public System {
  public:
    virtual void run(float dT) = 0;
};