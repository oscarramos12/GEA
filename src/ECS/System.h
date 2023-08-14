#pragma once

class Scene;

class System {
  public:
    void setScene(Scene& s) {
      scene = &s;
    }
    
  protected:
    Scene* scene = nullptr;
};
