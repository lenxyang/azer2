#pragma once

#include <memory>

#include "azer/render/blending.h"
#include "azer/render/entity.h"
#include "azer/render/rasterizer_state.h"

namespace azer {
class AmbientColorEffect;
class Camera;
class VertexDesc;
class FrustumBox {
 public:
  FrustumBox();
  void set_alpha(float v) { alpha_ = v;}
  void set_color(const Vector3& col) { color_ = col;}
  void Update(const Camera& camera, float zfar = -1.0f);
  void Render(const Camera& camera, Renderer* renderer);
 private:
  Vector3 color_;
  float alpha_;
  EntityPtr frustum_box_;
  scoped_refptr<AmbientColorEffect> color_effect_;
  RasterizerStatePtr rasterizer_ptr_;
  BlendingPtr blending_;
  DISALLOW_COPY_AND_ASSIGN(FrustumBox);
};

EntityDataPtr CreateFrustumBox(VertexDesc* desc, const Camera& camera);
}  // namespace azer
