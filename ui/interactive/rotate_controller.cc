#include "azer/ui/interactive/rotate_controller.h"

#include "base/logging.h"

namespace azer {
RotateController::RotateController(const Vector3& orientation) 
    : init_orientation_(initorientation) {
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
