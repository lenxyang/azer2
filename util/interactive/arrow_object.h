#pragma once


#include "azer/effect/color_effect.h"
#include "azer/effect/light.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"

namespace azer {
class Camera;
class Renderer;

class ArrowObject {
 public:
  ArrowObject();

  const Vector3& position() const { return position_;}
  void set_scale(const Vector3& v);
  void set_position(const Vector3& pos);
  void set_directional(const Vector3& dir);
  void set_orientation(const Quaternion& q);
  void set_color(const Vector4& c);
  void Render(const Camera& camera, Renderer* renderer);
 private:
  void OnParamUpdate();
  EntityPtr arrow_;
  Vector4 color_;
  Vector3 scale_;
  Vector3 position_;
  Quaternion orientation_;
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(ArrowObject);
};
}  // namespace azer
