#include "azer/render/renderable_object.h"

#include "base/logging.h"
#include "azer/render/blending.h"
#include "azer/render/effect_params_provider.h"

namespace azer {
RenderableObject::RenderableObject()
    : provider_(NULL),
      hold_provider_(true) {
  effects_.resize(kObjRTNum);
}

RenderableObject::~RenderableObject() {
  if (hold_provider_)
    delete provider_;
}

bool RenderableObject::HasBlending() const {
  return blending_.get() != NULL;
}

void RenderableObject::SetBlending(BlendingPtr blending) {
  blending_ = blending;
}

void RenderableObject::SetEffectParamsProvider(
    EffectParamsProvider* provider, bool hold) {
  provider_ = provider;
  hold_provider_ = hold;
}

void RenderableObject::SetEffect(int target, EffectPtr effect) {
  DCHECK_LT(target, kObjRTNum);
  effects_[target] = effect;
}

EffectPtr& RenderableObject::GetEffect(int target) {
  DCHECK_LT(target, kObjRTNum);
  return effects_[target];
}

void RenderableObject::Render(int target, Renderer* renderer) {
  EffectPtr effect = effects_[target];
  if (effect.get()) {
    provider_->Apply(effect.get());
    effect->Use(renderer);
    RenderObj(target, renderer);
  }
}
}  // namespace azer
