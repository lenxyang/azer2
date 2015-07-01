#include "azer/render/renderable_object.h"

#include "base/logging.h"
#include "azer/base/resource.h"
#include "azer/render/blending.h"
#include "azer/render/effect_params_provider.h"
#include "azer/scene/scene_stage.h"

namespace azer {
RenderableObject::RenderableObject()
    : Resource(kRenderableObject),
      provider_(NULL),
      hold_provider_(true) {
  effects_.resize(kSceneRenderStageNum);
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
  DCHECK_LT(target, kSceneRenderStageNum);
  effects_[target] = effect;
}

EffectPtr& RenderableObject::GetEffect(int target) {
  DCHECK_LT(target, kSceneRenderStageNum);
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

void RenderableObject::InstancedRender(int num, int target, Renderer* renderer) {
  EffectPtr effect = effects_[target];
  if (effect.get()) {
    provider_->Apply(effect.get());
    effect->Use(renderer);
    RenderInstancedObj(num, target, renderer);
  }
}

void RenderableObject::RenderInstancedObj(int num, int target, Renderer* renderer) {
  CHECK(false);
}
}  // namespace azer
