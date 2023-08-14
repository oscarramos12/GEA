#include <print.h>
#include "Game.h"

Game::Game(const char* name, int width, int height)
  : screen_width(width), screen_height(height) 
{
  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow(name, 0, 0, width, height, 0);
  renderer = SDL_CreateRenderer(window, 0, 0);

  lastFPSUpdate = 0;
  frameStartTimestamp = 0;
  frameEndTimestamp = 0;
  frameCountPerSecond = 0;
  FPS = 0.0;

  int maxFPS = 60;
  frameDuration = (1.0f/maxFPS) * 1000.0f;
  frameCount = 0;
}

Game::~Game() {
  SDL_DestroyWindow(window);
}

void Game::setScene(Scene* newScene) {
  currentScene = newScene;
  currentScene->setup();
}

void Game::frameStart() {
  print("---------- Frame: ", frameCount, "----------");
  frameStartTimestamp = SDL_GetTicks();

  if (frameEndTimestamp) {
    dT = (frameStartTimestamp - frameEndTimestamp) / 1000.0f;
  } else {
    dT = 0;
  }
}

void Game::frameEnd() {
  frameEndTimestamp = SDL_GetTicks();

  // Delay
  float actualFrameDuration = frameEndTimestamp - frameStartTimestamp;
  if (actualFrameDuration < frameDuration) {
    SDL_Delay(frameDuration - actualFrameDuration);
  }

  // FPS Calculation
  frameCount++;
  frameCountPerSecond++;
  Uint32 currentTime = SDL_GetTicks();
  Uint32 timeElapsed = currentTime - lastFPSUpdate;

  if (timeElapsed > 1000) {
    FPS = frameCountPerSecond / (timeElapsed/1000);
    lastFPSUpdate = currentTime;
    frameCountPerSecond = 0;
  }
  vprint(FPS);
  print("===================================");
}

void Game::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      isRunning = false;
    }
    currentScene->processEvents(event);
  }
}

void Game::update() {
  currentScene->update(dT);
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);

  // draw game
  currentScene->render(renderer);

  SDL_RenderPresent(renderer);
}

bool Game::running() {
  return isRunning;
}

void Game::run() {
  while (running()) {
    frameStart();
    handleEvents();
    update();
    render();
    
    frameEnd();
  }
}
