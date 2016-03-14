#pragma once

#include "base/basictypes.h"
#include "azer/math/quaternion.h"
#include "azer/effect/light.h"
#include "azer/ui/interactive/interactive_controller.h"

namespace azer {

class LightController : public InteractiveController {
 public:
  LightController(const Quaternion& orientation);
  ~LightController() override;

  int32 GetPicking(const Ray& ray, Vector3* pos) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  const Quaternion& position() const { return orientation_;}
 private:
  const Quaternion init_orientation_;
  const Quaternion orientation_;
  DISALLOW_COPY_AND_ASSIGN(LightController);
};

}  // namespace azer
