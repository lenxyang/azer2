#pragma once

#include "azer/render/renderer.h"

namespace azer {
class ScopedCullingMode {
 public:
  ScopedCullingMode(CullingMode mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetCullingMode();
    modified_ = (prev_mode_ != mode);
    if (modified_)
      renderer_->SetCullingMode(mode);
  }
  ~ScopedCullingMode() {
    if (modified_)
      renderer_->SetCullingMode(prev_mode_);
  }
 private:
  CullingMode prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedCullingMode);
};

class ScopedFillMode {
 public:
  ScopedFillMode(FillMode mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetFillMode();
    modified_ = (prev_mode_ != mode);
    if (modified_) {
      renderer_->SetFillMode(mode);
    }
  }
  ~ScopedFillMode() {
    if (modified_) {
      renderer_->SetFillMode(prev_mode_);
    }
  }
 private:
  FillMode prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedFillMode);
};

class ScopedFrontFace {
 public:
  ScopedFrontFace(FrontFace mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetFrontFace();
    modified_ = (prev_mode_ != mode);
    if (modified_)
      renderer_->SetFrontFace(mode);
  }
  ~ScopedFrontFace() {
    if (modified_)
      renderer_->SetFrontFace(prev_mode_);
  }
 private:
  FrontFace prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedFrontFace);
};
}  // namespace azer
