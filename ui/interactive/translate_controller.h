#pragma once

#include "base/basictypes.h"
#include "azer/ui/interactive/interactive_controller.h"

namespace azer {

class TranslateController : public InteractiveController {
 public:
  TranslateController();

  int32 GetPicking(const Ray& ray, Vector3* pos) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(TranslateController);
};

}  // namespace azer
