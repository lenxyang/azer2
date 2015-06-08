#include "azer/scene/scene_surroundings.h"

namespace azer {
SceneSurroundings::SceneSurroundings() 
    : camera_(NULL) {
}

void SceneSurroundings::reset() {
  lights_.clear();
  configs_.clear();
}

void SceneSurroundings::SetCamera(const Camera* camera) {
  camera_ = camera;
}

void SceneSurroundings::PushLight(LightPtr light) {
  lights_.push_back(light);
}

void SceneSurroundings::PopLight() {
  lights_.pop_back();
}

void SceneSurroundings::PushConfig(ConfigNodePtr node) {
  configs_.push_back(node);
}

void SceneSurroundings::PopConfig() {
  configs_.pop_back();
}
}
