#include "azer/ui/compositor/layer.h"

#include <algorithm>
#include "base/logging.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/point3_f.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/size_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/size_conversions.h"
#include "ui/gfx/transform.h"

#include "azer/ui/compositor/compositor.h"
#include "azer/ui/compositor/layer_tree_host.h"

namespace azer {
namespace compositor {

namespace {
const Layer* GetRoot(const Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}
}

Layer::Layer(LayerDelegate* delegate)
    : delegate_(delegate)
    , host_(NULL)
    , parent_(NULL)
    , visible_(true)
    , order_(0)
    , min_order_(0)
    , max_order_(0)
    , sorted_(false) {
}

Layer::~Layer() {
  FOR_EACH_OBSERVER(LayerObserver, observers_, OnLayerDestroying(this));
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
  OnChildrenOrderChanged();

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
      layer->RemoveFromParent();
      layer->parent_ = NULL;
      OnChildrenOrderChanged();
      return true;
    }
  }

  return false;
}

void Layer::SetVisible(bool visible) {
  visible_ = visible;
}

void Layer::RemoveFromParent() {
}

void Layer::OnChildrenOrderChanged() {
  sorted_ = false;
}

namespace {
class LayerSort {
 public:
  bool operator() (const Layer* l1, const Layer* l2) {
    return l1->order() < l2->order();
  }
};
}

void Layer::SortChildren() {
  if (sorted_) { return;}
  std::sort(children_.begin(), children_.end(), LayerSort());
  int index = 0;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    Layer* cur = (*iter);
    cur->SetOrder(++index);
  }

  min_order_ = 0;
  max_order_ = children_.size() + 1;
  sorted_ = true;
}

void Layer::SetOrder(int32 o) {
  if (parent()) {
    parent()->OnChildrenOrderChanged();
  }
}

void Layer::BringToTop(Layer* layer) {
  layer->SetOrder(min_order_++);
}

void Layer::BringToBottom(Layer* layer) {
  layer->SetOrder(max_order_++);
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
  DCHECK(host_ != NULL) << "before set bounds, please put the layer into layer_tree";
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
  gfx::Rect parent_bounds = (parent_ ? parent_->target_bounds() : bounds());
  target_bounds_ = gfx::IntersectRects(parent_bounds , bounds());
}

void Layer::CalcOverlayBounds() {
  const gfx::Rect& root_bounds = host_->root()->bounds();
  float x = (float)target_bounds_.x() / (float)root_bounds.width();
  float y = (float)target_bounds_.y() / (float)root_bounds.height();
  float width_percent = (float)target_bounds_.width() / (float)root_bounds.width();
  float height_percent = (float)target_bounds_.height() / (float)root_bounds.height();
  overlay_bounds_ = gfx::RectF(2.0f * x - 1.0f, 
                               2.0f * y - 1.0f,
                               width_percent * 2.0f,
                               height_percent * 2.0f);
}

void Layer::CalcTexBounds() {
  float tu = (float)(target_bounds_.x()  - bounds().x()) / (float)bounds().width();
  float tv = (float)(target_bounds_.y()  - bounds().y()) / (float)bounds().height();
  float width = (float)target_bounds_.width() / (float)bounds().width();
  float height = (float)target_bounds_.height() / (float)bounds().height();
  tex_bounds_ = gfx::RectF(tu, tv, width, height);
}

void Layer::AddObserver(LayerObserver* observer) {
  observers_.AddObserver(observer);
}

void Layer::RemoveObserver(LayerObserver* observer) {
  observers_.RemoveObserver(observer);
}
}  // namespace compositor
}  // namespace azer
