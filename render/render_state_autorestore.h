#pragma once

#include "azer/render/renderer.h"

namespace azer {
class AutoCullingMode {
 public:
  AutoCullingMode(CullingMode mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetCullingMode();
    modified_ = (prev_mode_ != mode);
    if (modified_)
      renderer_->SetCullingMode(mode);
  }
  ~AutoCullingMode() {
    if (modified_)
      renderer_->SetCullingMode(prev_mode_);
  }
 private:
  CullingMode prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(AutoCullingMode);
};

class AutoFillMode {
 public:
  AutoFillMode(FillMode mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetFillMode();
    modified_ = (prev_mode_ != mode);
    if (modified_) {
      renderer_->SetFillMode(mode);
    }
  }
  ~AutoFillMode() {
    if (modified_) {
      renderer_->SetFillMode(prev_mode_);
    }
  }
 private:
  FillMode prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(AutoFillMode);
};

class AutoFrontFace {
 public:
  AutoFrontFace(FrontFace mode, Renderer* renderer)
      : renderer_(renderer) {
    prev_mode_ = renderer->GetFrontFace();
    modified_ = (prev_mode_ != mode);
    if (modified_)
      renderer_->SetFrontFace(mode);
  }
  ~AutoFrontFace() {
    if (modified_)
      renderer_->SetFrontFace(prev_mode_);
  }
 private:
  FrontFace prev_mode_;
  bool modified_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(AutoFrontFace);
};
}  // namespace azer
