#include "azer/render/renderer.h"

#include "base/logging.h"
#include "azer/render/effect.h"
#include "azer/render/render_system.h"

namespace azer {
Renderer::Renderer(RenderSystem* rs) :
    render_system_(rs) {
  default_state_ = rs->CreateRenderState();
}

Renderer::~Renderer() {
}

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
