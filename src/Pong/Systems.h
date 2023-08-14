#pragma once
#include <print.h>
#include "Components.h"

#include "ECS/SystemTypes/SystemTypes.h"


class HelloWorldSystem : public SetupSystem {
  public:
    HelloWorldSystem() {
      print("Hello World Constructor");
    }

    ~HelloWorldSystem() {
      print("Hello World Destructor");
    }

    void run() {
      print("Hello World run!");
    }
};

class RectRenderSystem : public RenderSystem {
  void run(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

    const auto view = scene->r.view<TransformComponent, SizeComponent>();

    for (const auto entity : view) {
      const auto t = view.get<TransformComponent>(entity);
      const auto s = view.get<SizeComponent>(entity);


      int x = t.position.x;
      int y = t.position.y;
      int w = s.w;
      int h = s.h;

      SDL_Rect rect = { x, y, w, h};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
};

class MovementUpdateSystem : public UpdateSystem {
  public:
    MovementUpdateSystem(int screen_width, int screen_height) 
    : screen_width(screen_width), screen_height(screen_height) {}

    void run(float dT) {
      const auto view = scene->r.view<TransformComponent, SpeedComponent, SizeComponent>();

      for (const auto entity : view) {
        auto& t = view.get<TransformComponent>(entity);
        auto& s = view.get<SpeedComponent>(entity);
        auto& sz = view.get<SizeComponent>(entity);


        if (s.x == 0 && s.y == 0) {
          continue;
        }

        const int nx = t.position.x + s.x * dT;
        const int ny = t.position.y + s.y * dT;

        if (ny  <= 0) {
          s.y *= -1;
        }
        if (ny + sz.h >= screen_height) {
          s.y *= -1;
        }
        if (nx + sz.w > screen_width || nx < 0) {
          exit(1);
        }

        t.position.x = nx;
        t.position.y = ny;
      }
    }
  private:
    int screen_width;
    int screen_height;
};


class PlayerInputSystem : public EventSystem {
  void run(SDL_Event event) {
    scene->r.view<SpeedComponent, PlayerComponent>()
      .each(
        [&](const auto entity, 
            auto& s, 
            auto& p) {
          if (event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
              case SDLK_DOWN:
              if(p.playerID == 1)
                s.y = p.moveSpeed;
                break;
              case SDLK_UP:
              if(p.playerID == 1)
                s.y = -p.moveSpeed;
                break;
              case SDLK_w:
              if(p.playerID == 2)
                s.y = -p.moveSpeed;
                break;
              case SDLK_s:
              if(p.playerID == 2)
                s.y = p.moveSpeed;
                break;
            }
          }
          if (event.type == SDL_KEYUP) {
            s.y = 0;
          }
        }
      );
    }
};

class CollisionDetectionUpdateSystem : public UpdateSystem {
  void run(float dT) {
    scene->r.view<TransformComponent, SizeComponent, ColliderComponent>()
      .each(
        [&](const auto entity,
            auto& transformComponent,
            auto& sizeComponent,
            auto& colliderComponent
          ) {
            // cada entidad que tiene collider
            // AABB
            scene->r.view<TransformComponent, SpeedComponent, SizeComponent>()
            .each(
              [&](const auto entity2,
                  auto& transformComponent2,
                  auto& speedComponent2,
                  auto& sizeComponent2
                ) {
                  if (entity == entity2) {
                    // skip self collision
                    return;
                  }

                  SDL_Rect boxCol1 = {
                    static_cast<int>(transformComponent.position.x),
                    static_cast<int>(transformComponent.position.y),
                    sizeComponent.w,
                    sizeComponent.h
                  };

                  SDL_Rect boxCol2 = {
                    static_cast<int>(transformComponent2.position.x),
                    static_cast<int>(transformComponent2.position.y),
                    sizeComponent2.w,
                    sizeComponent2.h
                  };

                  if (SDL_HasIntersection(&boxCol1, &boxCol2)) {
                    colliderComponent.triggered = true;
                    colliderComponent.transferSpeed = speedComponent2.x;
                  }

              }
            );
        }
      );
    }
};

class BounceUpdateSystem : public UpdateSystem {
  void run(float dT) {
    scene->r.view<TransformComponent, SpeedComponent, ColliderComponent>()
      .each(
        [&](const auto entity,
            auto& transformComponent,
            auto& speedComponent,
            auto& colliderComponent
          ) {
            if (colliderComponent.triggered) {
              speedComponent.x *= -1.2;
              speedComponent.y += colliderComponent.transferSpeed;

              colliderComponent.triggered = false;
            }
          }
      );
    }
};
