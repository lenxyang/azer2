#include "azer/render/compositor/layer.h"

#include <algorithm>
#include "base/logging.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/point3_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/size_conversions.h"
#include "ui/gfx/transform.h"

#include "azer/render/compositor/compositor.h"
#include "azer/render/compositor/layer_tree_host.h"
#include "azer/render/compositor/bitmap_layer.h"
#include "azer/render/compositor/canvas_layer.h"
#include "azer/render/compositor/nodraw_layer.h"
#include "azer/render/compositor/renderer_layer.h"

namespace azer {
namespace compositor {

namespace {
const Layer* GetRoot(const Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}
}

scoped_refptr<Layer> Layer::CreateLayer(LayerType type) {
  switch (type) {
    case kNotDrawnLayer:
      return new NoDrawLayer();
    case kCanvasLayer:
      return new CanvasLayer();
    case kRendererLayer:
      return new RendererLayer();
    case kBitmapLayer: 
      return new BitmapLayer();
    default: NOTREACHED(); return NULL;
  }
}

Layer::Layer(LayerType type)
    : delegate_(NULL)
    , host_(NULL)
    , parent_(NULL)
    , visible_(true)
    , type_(type) {
}

Layer::~Layer() {
  FOR_EACH_OBSERVER(LayerObserver, observers_, OnLayerDestroying(this));
  observers_.Clear();
}

void Layer::SetTreeHost(LayerTreeHost* host) {
  DCHECK(host_ == NULL);
  host_ = host;
  if (host_) {
    this->OnAttachedtoTreeHost();
  }
}

void Layer::Add(scoped_refptr<Layer> layer) {
  DCHECK(host_ != NULL);
  layer->parent_ = this;
  layer->SetTreeHost(GetTreeHost());
  children_.push_back(layer);
  layer->OnStackingChanged();

  layer->AddObserver(host_);
  FOR_EACH_OBSERVER(LayerObserver, layer->observers_, OnLayerAttachedOnTree(layer));
}

bool Layer::Contains(scoped_refptr<Layer> other) {
  for (const Layer* parent = other.get(); parent; parent = parent->parent()) {
    if (parent == this)
      return true;
  }
  return false;
}

bool Layer::Remove(scoped_refptr<Layer> layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter).get() == layer.get()) {
      children_.erase(iter);
      DCHECK(layer->parent() == this);
      layer->OnRemoveFromParent();
      layer->parent_ = NULL;
      layer->OnStackingChanged();
      return true;
    }
  }

  return false;
}

void Layer::SetVisible(bool visible) {
  visible_ = visible;
}

void Layer::SetBoundsInternal(const gfx::Rect& new_bounds) { 
  bounds_ = new_bounds;
  OnBoundsChanged();
}

void Layer::OnParentBoundsChanged() {
  OnBoundsChanged();
}

void Layer::OnAttachedtoTreeHost() {
  OnBoundsChanged();
  ScheduleDraw();

  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->OnAttachedtoTreeHost();
  }
}

void Layer::OnBoundsChanged() {
  if (AttachedToTreeHost()) {
    CalcTargetBounds();
    CalcOverlayBounds();
    CalcTexBounds();

    for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
      Layer* i = (*iter);
      i->OnParentBoundsChanged();
    }

    FOR_EACH_OBSERVER(LayerObserver, observers_, OnLayerResized(this));
  }
}


void Layer::CalcTargetBounds() {
  gfx::Rect parent_bounds = (parent_ ? parent_->GetTargetBounds() : bounds());
  target_bounds_ = gfx::IntersectRects(parent_bounds , bounds());
}

void Layer::CalcOverlayBounds() {
  if (AttachedToTreeHost()) {
    const gfx::Rect& root_bounds = host_->root()->bounds();
    float width_percent = (float)target_bounds_.width() / (float)root_bounds.width();
    float height_percent = (float)target_bounds_.height() / (float)root_bounds.height();
    float x = (float)target_bounds_.x() / (float)root_bounds.width();
    float y = (float)target_bounds_.y() / (float)root_bounds.height() + height_percent;
    overlay_bounds_ = gfx::RectF(2.0f * x - 1.0f, 
                                 -2.0f * y + 1.0f,
                                 width_percent * 2.0f,
                                 height_percent * 2.0f);
  } else {
    overlay_bounds_ = gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f);
  }
}

void Layer::CalcTexBounds() {
  if (AttachedToTreeHost()) {
    float tu = (float)(target_bounds_.x()  - bounds().x()) / (float)bounds().width();
    float tv = (float)(target_bounds_.y()  - bounds().y()) / (float)bounds().height();
    float width = (float)target_bounds_.width() / (float)bounds().width();
    float height = (float)target_bounds_.height() / (float)bounds().height();
    tex_bounds_ = gfx::RectF(tu, tv, width, height);
  } else {
    tex_bounds_ = gfx::RectF(0.0f, 0.0f, 1.0f, 1.0f);
  }
}

void Layer::AddObserver(LayerObserver* observer) {
  observers_.AddObserver(observer);
}

void Layer::RemoveObserver(LayerObserver* observer) {
  observers_.RemoveObserver(observer);
}

void Layer::SetColor(SkColor color) {
  color_ = color;
}

bool Layer::SchedulePaint(const gfx::Rect& invalid_rect) {
  /*
  if (AttachedToTreeHost()) {
    gfx::Point pt(invalid_rect.origin());
    ConvertPointToLayer(GetTreeHost()->root(), this, &pt);
    gfx::Rect rect(pt, invalid_rect.size());
    // mark all layer interacted with rect 'needed render'
    for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
      const gfx::Rect& child_bounds = (*iter)->bounds();
      if (child_bounds.Intersects(rect)) {
        GetTreeHost()->SetLayerNeedRedraw(*iter);
        (*iter)->SchedulePaint(invalid_rect);
      }
    }
  }
  return true;
  */
  return true;
}

void Layer::ScheduleDraw() {
  if (AttachedToTreeHost()) {
    GetTreeHost()->SetLayerNeedRedrawHierarchy(this);
  }
}

bool Layer::AttachedToTreeHost() const {
  return (host_ != NULL);
}
}  // namespace compositor
}  // namespace azer
