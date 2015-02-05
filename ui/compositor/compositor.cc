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
  gfx::Size size(1,1);
  host_.reset(new azer::layers::LayerTreeHost(size));
}

Compositor::~Compositor() {
  host_.reset();
}

void Compositor::SetRootLayer(Layer* root_layer) {
  DCHECK(!host_->root());
  host_->SetRoot(root_layer->layer());
  root_layer->compositor_ = this;
  root_layer_ = root_layer;
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  DCHECK(host_.get());
  root_layer()->SetBounds(gfx::Rect(0, 0, size_in_pixel.width(),
                                    size_in_pixel.height()));
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

void Compositor::DoRender() {
  host_->Composite();
  azer::AppContext* app = azer::AppContext::GetInstance();
  if (visible_) {
    app->RenderUI(host_->GetCompositedTexture());
  }
}

void Compositor::ScheduleFullRedraw() {
  host_->SetNeedsRedraw();
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  host_->SetNeedsRedrawRect(damage_rect);
}

void Compositor::ScheduleLayerRedraw(Layer* layer) {
  host_->SetLayerNeedsRedraw(layer->layer());
}

}  // namespace ui
