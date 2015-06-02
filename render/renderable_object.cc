#include "azer/render/renderable_object.h"

#include "base/logging.h"
#include "azer/render/blending.h"
#include "azer/render/effect_params_provider.h"

namespace azer {
RenderableObject::RenderableObject()
    : use_blending_(false) {
  effect_.resize(kObjRTNum);
}

bool RenderableObject::HasBlending() const {
  return blending_.get() != NULL;
}

void RenderableObject::SetBlending(BlendingPtr blending) {
  blending_ = blending;
}

void RenderableObject::SetEffectParamsProvider(
    scoped_ptr<EffectParamsProvider> provider) {
  provider_ = provider.Pass();
}

void RenderableObject::SetEffect(int target, EffectPtr effect) {
  DCHECK_LT(target, kObjRTNum);
  effect_[target] = effect;
}

EffectPtr& RenderableObject::effect(int target) {
  DCHECK_LT(target, kObjRTNum);
  return effect_[target];
}

void RenderableObject::Render(int target, Renderer* renderer) {
  EffectPtr effect = effects_[target];
  if (effect.get()) {
    provider_->Apply(effect.get());
    RenderObj(target, renderer);
  }
}
}  // namespace azer