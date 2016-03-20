#pragma once

#include "base/basictypes.h"
#include "azer/math/quaternion.h" 
#include "azer/util/interactive/interactive_controller.h"

namespace azer {

class InteractiveContext;

class RotateController : public InteractiveController {
 public:
  explicit RotateController(InteractiveContext* ctx);
  ~RotateController() override;

  int32 GetPicking(const gfx::Point& pt) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  const Quaternion& position() const { return orientation_;}
 private:
  const Quaternion orientation_;
  DISALLOW_COPY_AND_ASSIGN(RotateController);
};

}  // namespace azer
