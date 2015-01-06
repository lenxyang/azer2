#include "azer/ui/compositor/layer.h"

#include <algorithm>
#include "base/logging.h"

namespace azer {
namespace ui {
Layer::Layer(Layer* parent)
    : visible_(true)
    , order_(0)
    , min_order_(0)
    , max_order_(0)
    , sorted_(false)
    , host_(parent->GetTreeHost())
    , parent_(parent) {
  parent_->Add(scoped_refptr<Layer>(this));
}

Layer::Layer(LayerTreeHost* host)
    : visible_(true)
    , order_(0)
    , min_order_(0)
    , max_order_(0)
    , sorted_(false)
    , host_(host)
    , parent_(NULL) {
}

Layer::~Layer() {
}

void Layer::Add(scoped_refptr<Layer>& layer) {
  DCHECK(!IsChild(layer));
  children_.push_back(layer);
  OnChildrenOrderChanged();
}

bool Layer::IsChild(scoped_refptr<Layer>& layer) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter).get() == layer.get()) {
      return true;
    }
  }

  return false;
}

bool Layer::Remove(scoped_refptr<Layer>& layer) {
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
  bool operator() (const scoped_refptr<Layer>& l1, const scoped_refptr<Layer>& l2) {
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
}  // namespace ui
}  // namespace azer
