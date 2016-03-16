#include "azer/util/interactive/translate_controller.h"

#include "base/logging.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/util/interactive/env.h"

namespace azer {
TranslateController::TranslateController(const Vector3& initpos) 
    : initpos_(initpos) {
}

TranslateController::~TranslateController() {
}

int32 TranslateController::GetPicking(const Ray& ray, Vector3* pos) {
  return 0;
}

void TranslateController::UpdateFrame(const FrameArgs& args) {
}

void TranslateController::RenderFrame(Renderer* renderer) {
}
}  // namespace azer
