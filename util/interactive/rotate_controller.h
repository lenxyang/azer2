#pragma once

#include "base/basictypes.h"
#include "azer/math/quaternion.h" 
#include "azer/util/interactive/interactive_controller.h"

namespace azer {

class InteractiveContext;

class RotateControlerObj {
 public:
  enum {
    kAxisX,
    kAxisY,
    kAxisZ,
    kRoundX,
    kRoundY,
    kRoundZ,
    kSphere,
    kSubsetCount,
  };
  RotateControlerObj();

  void SetColor(int32 index, const Vector4& c);
  void ResetColor();
 private:
  void InitEntity();
  void AppendAxisData(EntityData* data);
  void AppendRoundData(EntityData* data);
  Vector4 colors_[kSubsetCount];
  EntityPtr entity_;
  Vector3 scale_;
  scoped_refptr<AmbientColorEffect> ambient_effect_;
  scoped_refptr<ColorEffect> color_effect_;
  static const float kAxisLength; 
  DISALLOW_COPY_AND_ASSIGN(RotateControlerObj);
};

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
