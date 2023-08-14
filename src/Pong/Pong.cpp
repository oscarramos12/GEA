#include <print.h>

#include "Pong.h"
#include "Systems.h"

#include "ECS/Entity.h"

Pong::Pong(const char* name, int width, int height)
  : Game(name, width, height)
{
  Scene* gameplayScene = createGameplayScene();
  setScene(gameplayScene);
}

Pong::~Pong() {
  
}

Scene* Pong::createGameplayScene() {
  Scene* scene = new Scene("GAMEPLAY SCENE");

  Entity ball = scene->createEntity("BALL", 50, 50);
  ball.addComponent<SizeComponent>(20, 20);
  ball.addComponent<SpeedComponent>(200, 600);
  ball.addComponent<ColliderComponent>(false, 0);

  Entity paddle1 = scene->createEntity("PLAYER 1", (screen_width - 20), screen_height /2);
  paddle1.addComponent<SizeComponent>(20, 100);
  paddle1.addComponent<SpeedComponent>(0, 0);
  paddle1.addComponent<PlayerComponent>(300,1);

  Entity paddle2 = scene->createEntity("PLAYER 2", 0, screen_height /2);
  paddle2.addComponent<SizeComponent>(20, 100);
  paddle2.addComponent<SpeedComponent>(0, 0);
  paddle2.addComponent<PlayerComponent>(300,2);

  scene->addSetupSystem(new HelloWorldSystem());
  scene->addRenderSystem(new RectRenderSystem());
  scene->addUpdateSystem(new BounceUpdateSystem());
  scene->addUpdateSystem(new MovementUpdateSystem(screen_width, screen_height));
  scene->addEventSystem(new PlayerInputSystem());

  scene->addUpdateSystem(new CollisionDetectionUpdateSystem());

  return scene;
}
