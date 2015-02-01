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

Layer* Layer::CreateLayer(LayerType type) {
  switch (type) {
    case kNotDrawnLayer:
      return new NoDrawLayer();
    case kCanvasLayer:
      return new RendererLayer();
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
}

void Layer::Add(Layer* layer) {
  DCHECK(host_ != NULL);
  DCHECK(!IsChild(layer));
  layer->parent_ = this;
  layer->SetTreeHost(GetTreeHost());
  children_.push_back(layer);
  layer->OnStackingChanged();

  layer->AddObserver(host_);
  FOR_EACH_OBSERVER(LayerObserver, layer->observers_, OnLayerAttachedOnTree(layer));
}

bool Layer::IsChild(Layer* layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter) == layer) {
      return true;
    }
  }

  return false;
}

bool Layer::Remove(Layer* layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter) == layer) {
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

bool Layer::GetTargetTransformRelativeTo(const Layer* ancestor,
                                         gfx::Transform* transform) const {
  const Layer* p = this;
  for (; p && p != ancestor; p = p->parent()) {
    gfx::Transform translation;
    translation.Translate(static_cast<float>(p->bounds().x()),
                            static_cast<float>(p->bounds().y()));
    transform->ConcatTransform(translation);
  }
  return p == ancestor;
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

void Layer::SetBoundsInternal(const gfx::Rect& new_bounds) { 
  bounds_ = new_bounds;
  OnBoundsChanged();
}

void Layer::OnParentBoundsChanged() {
  OnBoundsChanged();
}

void Layer::OnBoundsChanged() {
  CalcTargetBounds();
  CalcOverlayBounds();
  CalcTexBounds();

  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    Layer* i = (*iter);
    i->OnParentBoundsChanged();
  }

  FOR_EACH_OBSERVER(LayerObserver, observers_, OnLayerResized(this));
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

void Layer::SetNeedRedraw(const gfx::Rect& rect) {
  // compute the children in rect or interact with rect
  host_->SetLayerNeedRedrawHierarchy(this);
}

void Layer::StackAtTop(Layer* child) {
  if (children_.size() <= 1 || child == children_.back())
    return;  // Already in front.
  StackAbove(child, children_.back());
}

void Layer::StackAtBottom(Layer* child) {
  if (children_.size() <= 1 || child == children_.front())
    return;  // Already on bottom.
  StackBelow(child, children_.front());
}

void Layer::StackAbove(Layer* child, Layer* other) {
  StackRelativeTo(child, other, true);
}

void Layer::StackBelow(Layer* child, Layer* other) {
  StackRelativeTo(child, other, false);
}

void Layer::StackRelativeTo(Layer* child, Layer* other, bool above) {
  DCHECK_NE(child, other);
  DCHECK_EQ(this, child->parent());
  DCHECK_EQ(this, other->parent());

  const size_t child_i =
      std::find(children_.begin(), children_.end(), child) - children_.begin();
  const size_t other_i =
      std::find(children_.begin(), children_.end(), other) - children_.begin();
  if ((above && child_i == other_i + 1) || (!above && child_i + 1 == other_i))
    return;

  const size_t dest_i =
      above ?
      (child_i < other_i ? other_i : other_i + 1) :
      (child_i < other_i ? other_i - 1 : other_i);
  children_.erase(children_.begin() + child_i);
  children_.insert(children_.begin() + dest_i, child);
}

void Layer::SetColor(SkColor color) {
  color_ = color;
}

bool Layer::SchedulePaint(const gfx::Rect& invalid_rect) {
  DCHECK(AttachedToTreeHost());
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
  return true;
}

void Layer::ScheduleDraw() {
  DCHECK(AttachedToTreeHost());
  GetTreeHost()->SetLayerNeedRedrawHierarchy(this);
}

bool Layer::AttachedToTreeHost() const {
  return (host_ != NULL);
}
}  // namespace compositor
}  // namespace azer
