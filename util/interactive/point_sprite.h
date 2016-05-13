#pragma once

#include "base/basictypes.h"
#include "azer/effect/light.h"
#include "azer/effect/color_effect.h"
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 

namespace azer {
class Camera;
class Renderer;
class PointSprite {
 public:
  PointSprite();
  const Vector3& position() const { return position_;}
  void set_scale(const Vector3& v) { scale_ = v;}
  void set_position(const Vector3& pos) { position_ = pos;}

  void Render(const Camera& camera, Renderer* renderer);
 private:
  Vector3 position_;
  Vector3 scale_;
  DISALLOW_COPY_AND_ASSIGN(PointSprite);
};
}  // namespace azer
