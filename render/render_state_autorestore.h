#pragma once

#include "azer/render/renderer.h"

namespace azer {
class AutoCullingMode {
 public:
  AutoCullingMode(CullingMode mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetCullingMode();
    renderer_->SetCullingMode(mode);
  }
  ~AutoCullingMode() {
    renderer_->SetCullingMode(prev_mode_);
  }
 private:
  CullingMode prev_mode_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(AutoCullingMode);
};
}  // namespace azer
