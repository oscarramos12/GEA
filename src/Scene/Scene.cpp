#include <print.h>

#include "Scene.h"

#include "ECS/Entity.h"
#include "ECS/Components.h"




Scene::Scene(const std::string& name)
  : name(name) {
    print("Scene Created: ", name);
}

Scene::~Scene() {
    print("Scene Destroyed");

    for (auto s : setupSystems) {
      delete s;  // call each system destructor
    }
}

Entity Scene::createEntity(const std::string& name, int x, int y) {
  Entity entity = { r.create(), this };
  entity.addComponent<NameComponent>(name);
  entity.addComponent<TransformComponent>(glm::vec2(x, y));

  return entity;
}

void Scene::addSetupSystem(SetupSystem* system) {
    system->setScene(*this);

    setupSystems.push_back(system);
}

void Scene::setup() {
  for (const auto& s : setupSystems) {
    s->run();
  }
}

void Scene::addEventSystem(EventSystem* system) {
    system->setScene(*this);
    eventSystems.push_back(system);
}

void Scene::processEvents(SDL_Event event) {
  for (const auto& s : eventSystems) {
    s->run(event);
  }
}

void Scene::addUpdateSystem(UpdateSystem* system) {
    system->setScene(*this);
    updateSystems.push_back(system);
}

void Scene::update(float dT) {
  for (const auto& s : updateSystems) {
    s->run(dT);
  }
}

void Scene::addRenderSystem(RenderSystem* system) {
    system->setScene(*this);
    renderSystems.push_back(system);
}

void Scene::render(SDL_Renderer* renderer) {
  for (const auto& s : renderSystems) {
    s->run(renderer);
  }
}