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
class DirLightControllerObj {
 public:
  DirLightControllerObj();

  void ResetColor();
 private:
  DISALLOW_COPY_AND_ASSIGN(DirLightControllerObj);
};
}  // namespace azer
