#pragma once


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/renderer.h"
#include "azer/render/common.h"

namespace azer {
class Renderer;
class AZER_EXPORT RasterizerState : public ::base::RefCounted<RasterizerState> {
 public:
  RasterizerState() {}
  virtual ~RasterizerState() {}

  virtual FillMode GetFillMode(void) = 0;
  virtual void SetFillMode(FillMode mode) = 0;
  virtual CullingMode GetCullingMode(void) = 0;
  virtual void SetCullingMode(CullingMode mode) = 0;
  virtual FrontFace GetFrontFace() = 0;
  virtual void SetFrontFace(FrontFace mode) = 0;
  virtual void EnableMultisampleAntiAliasing(bool enable) = 0;
  virtual bool IsMultisampleAntiAliasingEnabled() = 0;
  virtual void EnableLineAntialiasing(bool enable) = 0;
  virtual bool IsLineAntialiasingEnabled() = 0;

  virtual void Apply(Renderer* renderer) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(RasterizerState);
};

typedef scoped_refptr<RasterizerState> RasterizerStatePtr;

class ScopedRasterizerState {
 public:
  ScopedRasterizerState(Renderer* renderer)
      : renderer_(renderer) {
    prev_state_ = renderer->GetRasterizerState();;
  }
  ~ScopedRasterizerState() {
    renderer_->SetRasterizerState(prev_state_.get());
  }
 private:
  RasterizerStatePtr prev_state_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedRasterizerState);
};
}  // namespace azer
