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

namespace azer {
namespace compositor {

namespace {
const Layer* GetRoot(const Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}
}


Layer::Layer(Delegate* delegate)
    : delegate_(delegate)
    , parent_(NULL)
    , visible_(true)
    , order_(0)
    , min_order_(0)
    , max_order_(0)
    , sorted_(false)
    , parent_(parent) {
}


Layer::~Layer() {
}

void Layer::Add(Layer* layer) {
  DCHECK(!IsChild(layer));
  layer_->parent = this;
  children_.push_back(layer);
  OnChildrenOrderChanged();
}

bool Layer::IsChild(Layer* layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter).get() == layer.get()) {
      return true;
    }
  }

  return false;
}

bool Layer::Remove(Layer* layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter).get() == layer.get()) {
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
    Layer* cur = (*iter).get();
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

}  // namespace compositor
}  // namespace azer
