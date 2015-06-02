#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/blending.h"


namespace azer {

class FrameArgs;
class Renderer;
class EffectParamsProvider;

// Renderable object Render Target enum
enum {
  kObjRTShadowMap = 0,
  kObjRTDeferred,
  kObjRTNormal,
  kObjRTNum,
};

class AZER_EXPORT RenderableObject : public ::base::RefCounted<RenderableObject> {
 public:
  RenderableObject() {}
  virtual ~RenderableObject() {}

  EffectPtr& effect(int target);
  void SetEffect(int target, EffectPtr effect);
  void SetEffectParamsProvider(scoped_ptr<EffectParamsProvider> provider);

  void Render(int target, Renderer* renderer);
  bool HasBlending() const;
  void SetBlending(BlendingPtr blending);
  EffectParamsProvider* provider() { return provider_.get();}
 protected:
  void RenderObj(int target, Renderer* renderer);
 private:
  std::vector<EffectPtr> effects_;
  BlendingPtr blending_;
  scoped_ptr<EffectParamsProvider> provider_;
  DISALLOW_COPY_AND_ASSIGN(RenderableObject);
};

typedef scoped_refptr<RenderableObject> RenderableObjectPtr;
}  // namespace azer
