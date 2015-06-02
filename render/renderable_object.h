#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/effect_params_provider.h"

namespace azer {

class FrameArgs;

class AZER_EXPORT RenderableObject : public ::base::RefCounted<RenderableObject> {
 public:
  RenderableObject() {}
  virtual ~RenderableObject() {}

  virtual void Render(const FrameArgs& frame, Renderer* renderer) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(RenderableObject);
};

typedef scoped_refptr<RenderableObject> RenderableObjectPtr;
}  // namespace azer
