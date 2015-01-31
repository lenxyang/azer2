#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"

#include "azer/render/render_system.h"
#include "azer/render/compositor/compositor.h"
#include "azer/render/compositor/layer.h"
#include "azer/render/compositor/layer_tree_host.h"

namespace ui {

Compositor::Compositor(gfx::AcceleratedWidget widget) {
  CHECK(azer::RenderSystem::Current() == NULL);
  CHECK(azer::LoadRenderSystem(widget));
  host_.reset(new azer::compositor::LayerTreeHost(gfx::Size(1, 1)));
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
  host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(host_.get());
  host_->SetRoot(root_layer->layer());
  root_layer_->compositor_ = this;
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
}  // namespace ui
