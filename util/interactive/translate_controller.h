#pragma once

#include "base/basictypes.h"
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class Camera;
class Renderer;
class AmbientColorEffect;
class ColorEffect;

class TranslateControlObj {
 public:
  enum {
    kAxisX = 0,
    kAxisY,
    kAxisZ,
    kLineXY,
    kLineXZ,
    kLineYZ,
    kLineYX,
    kLineZX,
    kLineZY,
    kPlaneXY,
    kPlaneYZ,
    kPlaneZX,
    kSubsetCount,
  };

  TranslateControlObj();
  void SetScale(const Vector3& v) { scale_ = v;}
  void SetColor(int32 index, const Vector4& color) { colors_[index] = color;}
  void ResetColor();
  void Update(const Camera* camera, const Vector3& position);
  void Render(Renderer* renderer);
 private:
  EntityDataPtr InitAxesObjects(VertexDesc* desc);
  EntityDataPtr InitAxesConeData(VertexDesc* desc);
  Vector3 scale_;
  Vector4 colors_[kSubsetCount];
  EntityPtr entity_;
  EntityPtr cones_;
  scoped_refptr<AmbientColorEffect> ambient_effect_;
  scoped_refptr<ColorEffect> color_effect_;
  DISALLOW_COPY_AND_ASSIGN(TranslateControlObj);
};

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
