#pragma once

#include "base/basictypes.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/light.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class ColorEffect;
class Renderer;

class DirLightControllerObj {
 public:
  DirLightControllerObj();

  void SetColor(const Vector4& c) { color_ = c;}
  void Update(const Camera* camera);
  void Render(Renderer* renderer);
 private:
  EntityPtr arrow_;
  Vector4 color_;
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(DirLightControllerObj);
};
}  // namespace azer
