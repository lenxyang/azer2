#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {

namespace d3d11 {
class D3DRenderSystem;
}

class AZER_EXPORT RenderSystemCapability {
 public:
  bool HarewareTransformAndLightingSupported() const {
    return hw_transform_light_;
  }
 private:
  // Device can support transformation and lighting in hardware.
  bool hw_transform_light_;

  int32 max_simultaneous_textures_;
  friend class d3d11::D3DRenderSystem;
};
}  // namespace azer
