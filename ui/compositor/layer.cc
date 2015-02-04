#include "azer/ui/compositor/layer.h"

#include <algorithm>

#include "base/command_line.h"
#include "base/debug/trace_event.h"
#include "base/json/json_writer.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/animation/animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/display.h"
#include "ui/gfx/interpolated_transform.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/size_conversions.h"

namespace ui {

class CCLayerDelegate : public azer::compositor::LayerDelegate {
 public:
  CCLayerDelegate(ui::LayerDelegate* delegate)
      : delegate_(delegate) {
  }

  void OnPaintLayer(gfx::Canvas* canvas) override {
    if (delegate_) {
      delegate_->OnPaintLayer(canvas);
    }
  }
 protected:
  ui::LayerDelegate* delegate_;
};

namespace {
const Layer* GetRoot(const Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}
}  // namespace

Layer::Layer(LayerType type)
    : parent_(NULL)
    , delegate_(NULL)
    , type_(type)
    , background_blur_radius_(0)
    , compositor_(NULL)
    , owner_(NULL)
    , layer_(NULL)  {
  layer_ = CreateCCLayer(type);
  CHECK(layer_);
}

Layer::~Layer() {
}

azer::compositor::Layer* Layer::CreateCCLayer(LayerType type) {
  switch (type) {
    case LAYER_NOT_DRAWN:
      return azer::compositor::Layer::CreateLayer(azer::compositor::Layer::kNotDrawnLayer);
    case LAYER_TEXTURED:
      return azer::compositor::Layer::CreateLayer(azer::compositor::Layer::kCanvasLayer);
    default: CHECK(false) << "not support";
      return NULL;
  }
}

const Compositor* Layer::GetCompositor() const {
  return GetRoot(this)->compositor_;
}

void Layer::Add(Layer* child) {
if (child->parent_)
    child->parent_->Remove(child);
  child->parent_ = this;
  children_.push_back(child);

  layer_->Add(child->layer());
}

void Layer::Remove(Layer* child) {
  std::vector<Layer*>::iterator i =
    std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);
  child->parent_ = NULL;

  layer_->Remove(child->layer());
}

void Layer::StackAtTop(Layer* child) {
  if (children_.size() <= 1 || child == children_.back())
    return;  // Already in front.
  StackAbove(child, children_.back());
}

void Layer::StackAbove(Layer* child, Layer* other) {
  StackRelativeTo(child, other, true);
}

void Layer::StackAtBottom(Layer* child) {
  if (children_.size() <= 1 || child == children_.front())
    return;  // Already on bottom.
  StackBelow(child, children_.front());
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

void Layer::SetTransform(const gfx::Transform& transform) {
}

gfx::Transform Layer::GetTargetTransform() const {
  return gfx::Transform(); 
}

gfx::Transform Layer::transform() const {
  return gfx::Transform();
}

void Layer::SetBounds(const gfx::Rect& bounds) {
  if (layer_->bounds() == bounds) {
    return;
  }

  layer_->SetBounds(bounds);
  base::Closure closure;
  if (delegate_)
    closure = delegate_->PrepareForLayerBoundsChange();
  if (!closure.is_null())
    closure.Run();
}

const gfx::Rect& Layer::bounds() const {
  return layer_->bounds();
}

gfx::Rect Layer::GetTargetBounds() const {
  return layer_->GetTargetBounds();
}

void Layer::SetMasksToBounds(bool masks_to_bounds) {
}

bool Layer::GetMasksToBounds() const {
  return false;
}

float Layer::opacity() const {
  return 1.0f;
}

void Layer::SetOpacity(float opacity) {
}

float Layer::GetTargetOpacity() const {
  return opacity();
}

void Layer::SetVisible(bool visible) {
  layer_->SetVisible(visible);
}

bool Layer::visible() const {
  return layer_->visible();
}

bool Layer::GetTargetVisibility() const {
  return visible();
}

bool Layer::IsDrawn() const {
  return true;
}

void Layer::SetFillsBoundsOpaquely(bool fills_bounds_opaquely) {
}

bool Layer::fills_bounds_opaquely() const {
  return true;
}

void Layer::SetFillsBoundsCompletely(bool fills_bounds_completely) {
}

const std::string& Layer::name() const {
  return layer_->name();
}

void Layer::set_name(const std::string& name) {
  layer_->set_name(name);
}

void Layer::set_delegate(LayerDelegate* delegate) {
  delegate_ = delegate;
  cc_delegate_.reset(new CCLayerDelegate(delegate_));
  layer_->set_delegate(cc_delegate_.get());
}

bool Layer::SchedulePaint(const gfx::Rect& invalid_rect) {
  layer_->SchedulePaint(invalid_rect);
  return true;
}

void Layer::ScheduleDraw() {
  layer_->ScheduleDraw();
}

void Layer::CompleteAllAnimations() {
}

void Layer::SuppressPaint() {
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

void Layer::SetSubpixelPositionOffset(const gfx::Vector2dF offset) {
}

bool Layer::Contains(Layer* layer) {
  return layer_->Contains(layer->layer());
}

}  // namespace ui
