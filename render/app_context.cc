#include "azer/render/app_context.h"

#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {
AppContext* AppContext::context_ = NULL;

AppContext* AppContext::CreateInstance(gfx::AcceleratedWidget widget) {
  CHECK(!context_);
  context_ = new AppContext(widget);
  return context_;
}

AppContext* AppContext::GetInstance() {
  CHECK(context_);
  return context_;
}

void AppContext::DestroyInstance() {
  CHECK(context_);
  delete context_;
  context_ = NULL;
}

AppContext::AppContext(gfx::AcceleratedWidget widget) 
    : render_system_(NULL) {
  surface_.reset(new azer::Surface(widget));
  render_system_ = azer::RenderSystem::Current();
  CHECK(render_system_);
  swapchain_.reset(render_system_->CreateSwapChainForSurface(surface_.get()));
  renderer_ = swapchain_->GetRenderer();
  overlay_.reset(render_system_->CreateOverlay());
}

AppContext::~AppContext() {
}


void AppContext::RenderUI(TexturePtr& texture) {
  overlay_->SetTexture(texture);
  overlay_->SetBounds(gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f));
  overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));
  overlay_->Render(renderer_.get());
}
}  // namespace azer
