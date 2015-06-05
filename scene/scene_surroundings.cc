#include "azer/scene/scene_surroundings.h"

namespace azer {
SceneSurroundings::SceneSurroundings() {
}

void SceneSurroundings::reset() {
  while (!lights_.empty()) {
    lights_.pop();
  }
}

void SceneSurroundings::SetCamera(const Camera* camera) {
  camera_ = camera;
}

void SceneSurroundings::PushLight(LightPtr light) {
  lights_.push(light);
}

void SceneSurroundings::PopLight() {
  lights_.pop();
}

void SceneSurroundings::PushConfig(ConfigNodePtr node) {
  
}

void SceneSurroundings::PopConfig() {
}
}
