#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/render_system_enum.h"

namespace azer {
class Renderer;
class AZER_EXPORT RenderState : public ::base::RefCounted<RenderState> {
 public:
  RenderState() {}
  virtual ~RenderState() {}

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

  // depth buffer state
  virtual void EnableDepthTest(bool enable) = 0;
  virtual bool IsDepthTestEnabled()  = 0;
  virtual void SetDepthCompareFunc(CompareFunc::Type func) = 0;

  virtual void Apply(Renderer* renderer) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(RenderState);
};

typedef scoped_refptr<RenderState> RenderStatePtr;
}  // namespace azer
