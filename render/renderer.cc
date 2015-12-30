#include "azer/render/renderer.h"

#include "base/logging.h"
#include "azer/render/effect.h"

namespace azer {

void Renderer::UseEffect(Effect* effect) {
  effect->Apply(this);
}

void Renderer::ResetRenderState() {
  DCHECK(default_state_.get());
  SetRenderState(default_state_);
}

void Renderer::SetRenderState(RenderState* render_state) {
  current_state_ = render_state;
  current_state_->Apply(this);
}

RenderState* Renderer::GetRenderState() {
  return current_state_;
}
}  // namespace azer
