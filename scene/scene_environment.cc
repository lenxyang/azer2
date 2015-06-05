#include "studio/scene/scene_environment.h"

namespace azer {
SceneEnvironment::SceneEnvironment() {
}

void SceneEnvironment::reset() {
  while (!lights_.empty()) {
    lights_.pop();
  }
}

void SceneEnvironment::SetCamera(const Camera* camera) {
  camera_ = camera;
}

void SceneEnvironment::PushLight(LightPtr light) {
  lights_.push(light);
}

void SceneEnvironment::PopLight() {
  lights_.pop();
}
}
