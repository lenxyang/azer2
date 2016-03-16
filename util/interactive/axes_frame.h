#pragma once

#include "azer/math/vector3.h"
#include "azer/math/vector4.h"
#include "azer/render/entity.h"
#include "azer/effect/color_effect.h"

namespace azer {
class Camera;
class AxesFrame {
 public:
  AxesFrame();
  
  void Update(const Camera* camera, const Vector3& position);
  void SetColor(int32 index, const Vector4& color);
  void Render(Renderer* renderer);
 private:
  Vector3 scale_;
  Vector4 colors_[3];
  scoped_refptr<AmbientColorEffect> effect_;
  EntityPtr entity_;
  DISALLOW_COPY_AND_ASSIGN(AxesFrame);
};

class ConeAxesFrame {
 public:
  ConeAxesFrame();
  
  void Update(Camera* camera);
  void SetColor(int32 index, const Vector3& color);
  void Render(Renderer* renderer);
 private:
  Vector4 colors_[3];
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(ConeAxesFrame);
};
}  // namespace azer
