#pragma once

#include "base/basictypes.h"
#include "azer/math/quaternion.h" 
#include "azer/util/interactive/interactive_controller.h"

namespace azer {

class RotateController : public InteractiveController {
 public:
  RotateController(const Quaternion& orientation);
  ~RotateController() override;

  int32 GetPicking(const Ray& ray, Vector3* pos) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  const Quaternion& position() const { return orientation_;}
 private:
  const Quaternion init_orientation_;
  const Quaternion orientation_;
  DISALLOW_COPY_AND_ASSIGN(RotateController);
};

}  // namespace azer
