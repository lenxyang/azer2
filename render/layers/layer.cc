#include "azer/render/layers/layer.h"

#include <algorithm>
#include "base/logging.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/point3_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/size_conversions.h"
#include "ui/gfx/transform.h"

#include "azer/render/layers/layer_tree_host.h"
#include "azer/render/layers/bitmap_layer.h"
#include "azer/render/layers/canvas_layer.h"
#include "azer/render/layers/nodraw_layer.h"
#include "azer/render/layers/renderer_layer.h"

namespace azer {
namespace layers {

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
    , parent_(NULL)
    , layer_tree_host_(NULL)
    , visible_(true)
    , type_(type) {
}

Layer::~Layer() {
}

void Layer::AddChild(scoped_refptr<Layer> child) {
  InsertChild(child, children_.size());
}

bool Layer::HasAncestor(scoped_refptr<Layer> other) {
  for (const Layer* parent = other.get(); parent; parent = parent->parent()) {
    if (parent == this)
      return true;
  }
  return false;
}

void Layer::SetLayerTreeHost(LayerTreeHost* host) {
  if (layer_tree_host_ == host)
    return;

  layer_tree_host_ = host;
}

void Layer::SetParent(Layer* layer) {
  parent_ = layer;
  SetLayerTreeHost(parent_ ? parent_->layer_tree_host() : nullptr);
}

void Layer::InsertChild(scoped_refptr<Layer> child, size_t index) {
  child->RemoveFromParent();
  child->SetParent(this);
  

  index = std::min(index, children_.size());
  children_.insert(children_.begin() + index, child);
  child->SetNeedsRedrawRecusive();

  if (layer_tree_host_) {
    child->layer_tree_host_ = layer_tree_host_;
    child->OnAttachedLayerTreeHost();
  }
}

void Layer::RemoveFromParent() {
  if (parent_)
    parent_->RemoveChildOrDependent(this);
}

void Layer::RemoveChildOrDependent(Layer* child) {
  for (LayerList::iterator iter = children_.begin();
       iter != children_.end();
       ++iter) {
    if (iter->get() != child)
      continue;

    child->SetParent(nullptr);
    children_.erase(iter);
    return;
  }
}

void Layer::SetTransform(const gfx::Transform& transform) {
}

gfx::Transform Layer::GetTargetTransform() const {
  return gfx::Transform(); 
}

gfx::Transform Layer::transform() const {
  return gfx::Transform();
}

void Layer::SetVisible(bool visible) {
  visible_ = visible;
}

void Layer::SetHideLayerAndSubtree(bool visible) {
  SetVisible(visible);
}

void Layer::SetBoundsInternal(const gfx::Rect& new_bounds) { 
  bounds_ = new_bounds;
  OnBoundsChanged();
}

void Layer::OnParentBoundsChanged() {
  OnBoundsChanged();
}

void Layer::OnBoundsChanged() {
  if (layer_tree_host()) {
    CalcTargetBounds();
    CalcOverlayBounds();
    CalcTexBounds();

    for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
      Layer* i = (*iter);
      i->OnParentBoundsChanged();
    }
  }
}


void Layer::CalcTargetBounds() {
  gfx::Rect parent_bounds = (parent_ ? parent_->GetTargetBounds() : bounds());
  Layer* cur = parent_;
  gfx::Point origin = bounds().origin();
  while (cur) {
    origin.set_x(origin.x() + cur->bounds().origin().x());
    origin.set_y(origin.y() + cur->bounds().origin().y());
    cur = cur->parent_;
  }
  target_bounds_ = gfx::Rect(origin, bounds_.size());
  target_bounds_ = gfx::IntersectRects(parent_bounds , target_bounds_);
}

void Layer::CalcOverlayBounds() {
  if (layer_tree_host()) {
    const gfx::Rect& root_bounds = layer_tree_host()->root()->bounds();
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
  if (layer_tree_host()) {
    float tu = 0.0f;
    float tv = 0.0f;
    float width = (float)target_bounds_.width() / (float)bounds().width();
    float height = (float)target_bounds_.height() / (float)bounds().height();
    tex_bounds_ = gfx::RectF(tu, tv, width, height);
  } else {
    tex_bounds_ = gfx::RectF(0.0f, 0.0f, 1.0f, 1.0f);
  }
}

void Layer::SetColor(SkColor color) {
  color_ = color;
}

// static
void Layer::ConvertPointToLayer(const Layer* source,
                                const Layer* target,
                                gfx::Point* point) {
  if (source == target)
    return;

  const Layer* root_layer = GetRoot(source);
  CHECK_EQ(root_layer, GetRoot(target));

  if (source != root_layer)
    source->ConvertPointForAncestor(root_layer, point);
  if (target != root_layer)
    target->ConvertPointFromAncestor(root_layer, point);
}

bool Layer::ConvertPointForAncestor(const Layer* ancestor,
                                    gfx::Point* point) const {
  gfx::Transform transform;
  bool result = GetTargetTransformRelativeTo(ancestor, &transform);
  gfx::Point3F p(*point);
  transform.TransformPoint(&p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

bool Layer::ConvertPointFromAncestor(const Layer* ancestor,
                                     gfx::Point* point) const {
  gfx::Transform transform;
  bool result = GetTargetTransformRelativeTo(ancestor, &transform);
  gfx::Point3F p(*point);
  transform.TransformPointReverse(&p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

bool Layer::GetTargetTransformRelativeTo(const Layer* ancestor,
                                         gfx::Transform* transform) const {
  const Layer* p = this;
  for (; p && p != ancestor; p = p->parent()) {
    gfx::Transform translation;
    translation.Translate(static_cast<float>(p->bounds().x()),
                          static_cast<float>(p->bounds().y()));
    // Use target transform so that result will be correct once animation is
    // finished.
    if (!p->GetTargetTransform().IsIdentity())
      transform->ConcatTransform(p->GetTargetTransform());
    transform->ConcatTransform(translation);
  }
  return p == ancestor;
}

void Layer::SetNeedsRedrawRecusive() {
  if (layer_tree_host()) {
    scoped_refptr<Layer> thisptr(this);
    layer_tree_host_->SetLayerNeedsRedrawRecusive(thisptr);
  }
}

void Layer::OnAttachedLayerTreeHost() {
  DCHECK(layer_tree_host_);
  OnBoundsChanged();
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->layer_tree_host_ = layer_tree_host_;
    (*iter)->OnBoundsChanged();
  }
}
}  // namespace layers
}  // namespace azer
