#include "azer/render/renderer.h"

#include "base/logging.h"
#include "azer/render/effect.h"
#include "azer/render/renderer.h"
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
  default_rasterizer_state_ = NULL;
  default_depth_state_ = NULL;
  current_depth_state_ = NULL;
  depth_ = NULL;
  targets_.clear();
}

void Renderer::BindEffect(Effect* effect) {
  effect->Apply(this);
}

void Renderer::ResetRasterizerState() {
  DCHECK(default_rasterizer_state_.get());
  SetRasterizerState(default_rasterizer_state_.get());
}

void Renderer::SetRasterizerState(RasterizerState* state) {
  current_rasterizer_state_ = state;
  current_rasterizer_state_->Apply(this);
}

RasterizerState* Renderer::GetRasterizerState() {
  return current_rasterizer_state_.get();
}

void Renderer::ResetDepthStencilState() {
  DCHECK(default_depth_state_.get());
  SetDepthStencilState(default_depth_state_.get(), 0);
}

void Renderer::SetDepthStencilState(DepthStencilState* state,
                                    uint32_t stencilref) {
  stencilref_ = stencilref;
  current_depth_state_ = state;
  current_depth_state_->Apply(this, stencilref);
}

DepthStencilState* Renderer::GetDepthStencilState() {
  return current_depth_state_.get();
}

void Renderer::SetDepthBuffer(DepthBuffer* depth) {
  DCHECK(size() == depth->size());
  depth_ = depth;
}

void Renderer::SetRenderTargets(std::vector<RenderTargetPtr>* targets) {
  targets_.clear();
  targets_ = *targets;
}

gfx::Size Renderer::size() const {
  if (!targets_.empty()) {
    return targets_[0]->size();
  } else {
    CHECK(depth_.get() != NULL);
    return depth_->size();
  }
}

RendererPtr CreateCommonRenderer(const gfx::Size& size) {
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options rdopt, depthopt;
  rdopt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  depthopt.size.width = rdopt.size.width = size.width();
  depthopt.size.height = rdopt.size.height = size.height();
  depthopt.size.depth = rdopt.size.depth = 1;
  depthopt.target = kBindTargetDepthStencil;
  depthopt.format = TexFormat::kD24UNormS8Uint;
  Texture2DPtr rttex = rs->CreateTexture2D(rdopt, NULL);
  Texture2DPtr depthtex = rs->CreateTexture2D(depthopt, NULL);
  DCHECK(rttex.get() && depthtex.get());
  RenderTargetPtr render_target = rs->CreateRenderTarget(
      RenderTarget::Options(), rttex.get());
  DepthBufferPtr depthbuffer = rs->CreateDepthBuffer(
      DepthBuffer::Options(), depthtex.get());
  RenderTargetPtrs targets;
  targets.push_back(render_target);
  RendererPtr renderer = rs->CreateRenderer(&targets, depthbuffer.get());
  DCHECK(renderer.get());
  return renderer;
}
}  // namespace azer
