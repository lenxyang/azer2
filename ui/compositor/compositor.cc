#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"

#include "azer/render/render_system.h"
#include "azer/render/compositor/compositor.h"
#include "azer/render/compositor/layer.h"
#include "azer/render/compositor/layer_tree_host.h"

namespace ui {

Compositor::Compositor(gfx::AcceleratedWidget widget) 
    : root_layer_(NULL) {
  gfx::Size size(1, 1);
  CHECK(azer::RenderSystem::Current() != NULL);
  surface_.reset(new azer::Surface(widget));
  azer::RenderSystem* render_system = azer::RenderSystem::Current();
  swapchain_.reset(render_system->CreateSwapChainForSurface(surface_.get()));
  renderer_ = swapchain_->GetRenderer();
  overlay_.reset(render_system->CreateOverlay());

  host_.reset(new azer::compositor::LayerTreeHost(size));
  compositor_.reset(new azer::compositor::Compositor);
}

Compositor::~Compositor() {
  compositor_.reset();
  host_.reset();
  azer::UnloadRenderSystem();
}

void Compositor::ScheduleFullRedraw() {
  compositor_->ScheduleDraw();
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  compositor_->ScheduleRedrawRect(damage_rect);
}

void Compositor::SetRootLayer(Layer* root_layer) {
  DCHECK(!host_->root());
  host_->SetRoot(root_layer->layer());
  host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(host_.get());
  root_layer->compositor_ = this;
  root_layer_ = root_layer;
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  DCHECK(host_.get());
  host_->resize(size_in_pixel);
}

void Compositor::SetBackgroundColor(SkColor color) {
}

gfx::Size Compositor::size() const { 
  return compositor_->size();
}

azer::SwapChainPtr& Compositor::GetSwapChain() { 
  return swapchain_;
}

void Compositor::DoComposite() {
  compositor_->DoDraw();
  compositor_->DoComposite();

  renderer_->Use();
  renderer_->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer_->ClearDepthAndStencil();

  overlay_->SetTexture(compositor_->GetOutputTexture());
  overlay_->SetBounds(gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f));
  overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));
  overlay_->Render(renderer_.get());
}
}  // namespace ui
