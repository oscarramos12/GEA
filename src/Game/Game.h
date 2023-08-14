#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Scene/Scene.h"

class Game {
  public:
    Game(const char* name, int width, int height);
    ~Game();

    void frameStart();
    void frameEnd();
    void handleEvents();
    void update();
    void render();
    bool running();

    void run();

    void setScene(Scene* newScene);

  protected:
    int width;
    int height;
    bool isRunning = true;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int screen_width;
    int screen_height;

    // FPS
    double FPS;
    int frameCount;
    int frameCountPerSecond;
    Uint32 frameStartTimestamp;
    Uint32 frameEndTimestamp;
    Uint32 lastFPSUpdate;
    float frameDuration;

    // delta time
    float dT;

    // current scene
    Scene* currentScene;
};
