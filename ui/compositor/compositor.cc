#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"

#include "azer/render/render_system.h"
#include "azer/render/app_context.h"
#include "azer/render/context2d.h"
#include "azer/render/layers/layer.h"
#include "azer/render/layers/layer_tree_host.h"


namespace ui {

Compositor::Compositor(gfx::AcceleratedWidget widget)
    : widget_(widget)
    , visible_(true)
    , root_layer_(NULL) {
  azer::AppContext::CreateInstance(widget);
  host_.reset(new azer::layers::LayerTreeHost());
}

Compositor::~Compositor() {
  host_.reset();
  azer::AppContext::DestroyInstance();
}

void Compositor::SetRootLayer(Layer* root_layer) {
  DCHECK(!host_->root());
  host_->SetRoot(root_layer->layer());
  root_layer->compositor_ = this;
  root_layer_ = root_layer;
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  DCHECK(host_.get());
  host_->SetScaleAndSize(scale, size_in_pixel);
}

void Compositor::SetBackgroundColor(SkColor color) {
  host_->SetBackgroundColor(color);
}

gfx::Size Compositor::size() const { 
  if (root_layer()) {
    return root_layer()->bounds().size();
  } else {
    return gfx::Size();
  }
}

azer::TexturePtr& Compositor::GetCompositedTexture() {
  return host_->GetCompositedTexture();
}

void Compositor::DoComposite() {
  host_->Composite();
}

void Compositor::ScheduleFullRedraw() {
  host_->SetNeedsRedraw();
}

void Compositor::FinishAllRendering() {
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  host_->SetNeedsRedrawRect(damage_rect);
}

void Compositor::ScheduleLayerRedraw(Layer* layer) {
  host_->SetLayerNeedsRedraw(layer->layer());
}

void Compositor::SetHostHasTransparentBackground(bool host_has_transparent_background) {
}
}  // namespace ui
