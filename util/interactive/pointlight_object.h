#pragma once

#include "azer/effect/effect.h"
#include "azer/render/blending.h"
#include "azer/util/interactive/graypoint.h"
#include "azer/util/geometry/sphere.h"

namespace azer {

class ColorEffect;

class PointLightObject {
 public:
  PointLightObject();
  ~PointLightObject();

  void set_position(const Vector3& pos) { position_ = pos;}
  void set_scale(const Vector3& v) { scale_ = v;}
  void set_color(const Vector4& c) { color_ = c;}
  void set_range(float r) { range_ = r;}
  void set_alpha(float a) { alpha_ = a;}
  void set_show_range(bool b) { show_range_ = b;}
  void Render(const Camera& camera, Renderer* renderer);
 private:
  Vector3 position_;
  Vector3 scale_;
  Vector4 color_;
  float range_, alpha_;
  bool show_range_;
  std::unique_ptr<GrayPoint> graypoint_;
  EntityPtr entity_;
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(PointLightObject);
};
}  // namespace azer
