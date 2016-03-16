#include "azer/util/interactive/rotate_controller.h"

#include "base/logging.h"

namespace azer {
RotateController::RotateController(const Quaternion& orientation) 
    : init_orientation_(orientation) {
}

RotateController::~RotateController() {
}

int32 RotateController::GetPicking(const Ray& ray, Vector3* pos) {
  return 0;
}

void RotateController::UpdateFrame(const FrameArgs& args) {
}

void RotateController::RenderFrame(Renderer* renderer) {
}
}  // namespace azer
