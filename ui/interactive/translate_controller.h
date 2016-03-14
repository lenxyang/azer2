#pragma once

#include "base/basictypes.h"
#include "azer/math/vector3.h" 
#include "azer/ui/interactive/interactive_controller.h"

namespace azer {

class TranslateController : public InteractiveController {
 public:
  TranslateController(const Vector3& initpos);
  ~TranslateController() override;

  int32 GetPicking(const Ray& ray, Vector3* pos) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  const Vector3& position() const { return position_;}
 private:
  const Vector3 initpos_;
  const Vector3 position_;
  DISALLOW_COPY_AND_ASSIGN(TranslateController);
};

}  // namespace azer
