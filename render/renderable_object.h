#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/blending.h"
#include "azer/render/effect.h"

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

class AZER_EXPORT RenderableObject : public Resource {
 public:
  RenderableObject();
  virtual ~RenderableObject();

  EffectPtr& GetEffect(int target);
  void SetEffect(int target, EffectPtr effect);
  void SetEffectParamsProvider(EffectParamsProvider* provider, bool hold = true);

  void Render(int target, Renderer* renderer);
  bool HasBlending() const;
  void SetBlending(BlendingPtr blending);
  EffectParamsProvider* provider() { return provider_;}
 protected:
  virtual void RenderObj(int target, Renderer* renderer) = 0;
 private:
  std::vector<EffectPtr> effects_;
  BlendingPtr blending_;
  EffectParamsProvider* provider_;
  bool hold_provider_;
  DISALLOW_COPY_AND_ASSIGN(RenderableObject);
};

typedef scoped_refptr<RenderableObject> RenderableObjectPtr;
}  // namespace azer
