#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"
#include "azer/render/compositor/compositor.h"
#include "azer/render/compositor/layer.h"
#include "azer/render/compositor/layer_tree_host.h"

namespace ui {

Compositor::Compositor(gfx::AcceleratedWidget widget) {
  // host_.reset(new azer::compositor::LayerTreeHost(bounds_.size()));
  // compositor_.reset(new azer::compositor::Compositor);
}

Compositor::~Compositor() {
}

void Compositor::ScheduleFullRedraw() {
  // compositor_->ScheduleFullRedraw();
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  compositor_->ScheduleRedrawRect(damage_rect);
}

void Compositor::SetRootLayer(Layer* root_layer) {
  DCHECK(!host_->root());
  host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(host_.get());
  host_->SetRoot(root_layer->layer());
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
}
}  // namespace ui
