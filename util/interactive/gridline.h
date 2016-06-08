#pragma once

#include "base/memory/ref_counted.h"
#include "azer/effect/effect.h"
#include "azer/effect/color_effect.h"
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/math/matrix4.h" 
#include "azer/render/entity.h"

namespace azer {

class Camera;
class Renderer;

class Gridline {
 public:
  Gridline(int32_t row, int32_t column, float cell = 1.0f);
  ~Gridline();
  
  void Update(const Camera& camera);
  void Render(Renderer* rs);
  void set_color(const Vector4& col) { color_ = col;}
  void set_position(const Vector3& pos) { position_ = pos;}
 private:
  scoped_refptr<AmbientColorEffect> effect_;
  Vector3 position_;
  Vector4 color_;
  Matrix4 world_;
  Matrix4 pv_;
  EntityPtr entity_;
  DISALLOW_COPY_AND_ASSIGN(Gridline);
};
}  // namespace azer
