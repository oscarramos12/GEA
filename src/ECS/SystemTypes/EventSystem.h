#pragma once
#include <SDL2/SDL.h>
#include "ECS/System.h"

class EventSystem : public System {
  public:
    virtual void run(SDL_Event event) = 0;
};