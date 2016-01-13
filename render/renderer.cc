#include "azer/render/renderer.h"

#include "base/logging.h"
#include "azer/render/effect.h"
#include "azer/render/render_system.h"

namespace azer {
Renderer::Renderer(RenderSystem* rs) :
    render_system_(rs) {
  default_state_ = rs->CreateRasterizerState();
}

Renderer::~Renderer() {
}

void Renderer::UseEffect(Effect* effect) {
  effect->Apply(this);
}

void Renderer::ResetRasterizerState() {
  DCHECK(default_state_.get());
  SetRasterizerState(default_state_);
}

void Renderer::SetRasterizerState(RasterizerState* render_state) {
  current_state_ = render_state;
  current_state_->Apply(this);
}

RasterizerState* Renderer::GetRasterizerState() {
  return current_state_;
}
}  // namespace azer
