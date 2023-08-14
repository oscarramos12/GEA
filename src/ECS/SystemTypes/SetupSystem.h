#pragma once
#include "ECS/System.h"

class SetupSystem : public System {
  public:
    virtual void run() = 0;
};