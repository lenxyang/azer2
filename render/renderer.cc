#include "azer/render/renderer.h"

#include "base/logging.h"
#include "azer/effect/effect.h"
#include "azer/render/render_system.h"
#include "azer/render/rasterizer_state.h"
#include "azer/render/depth_buffer.h"

namespace azer {
Renderer::Renderer(RenderSystem* rs) :
    render_system_(rs),
    stencilref_(0) {
  default_rasterizer_state_ = rs->CreateRasterizerState();
  default_depth_state_ = rs->CreateDepthStencilState();
}

Renderer::~Renderer() {
}

void Renderer::BindEffect(Effect* effect) {
  effect->Apply(this);
}

void Renderer::ResetRasterizerState() {
  DCHECK(default_rasterizer_state_.get());
  SetRasterizerState(default_rasterizer_state_);
}

void Renderer::SetRasterizerState(RasterizerState* state) {
  current_rasterizer_state_ = state;
  current_rasterizer_state_->Apply(this);
}

RasterizerState* Renderer::GetRasterizerState() {
  return current_rasterizer_state_;
}

void Renderer::ResetDepthStencilState() {
  DCHECK(default_depth_state_.get());
  SetDepthStencilState(default_depth_state_, 0);
}

void Renderer::SetDepthStencilState(DepthStencilState* state,
                                    uint32 stencilref) {
  stencilref_ = stencilref;
  current_depth_state_ = state;
  current_depth_state_->Apply(this, stencilref);
}

DepthStencilState* Renderer::GetDepthStencilState() {
  return current_depth_state_;
}
}  // namespace azer
