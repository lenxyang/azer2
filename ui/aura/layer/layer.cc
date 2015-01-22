#include "azer/ui/aura/layer/layer.h"

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

const Layer* GetRoot(const Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}

Layer::Layer() {
}

Layer::Layer(LayerType type) {
}

Layer::~Layer() {
  if (parent_) {
    parent_->Remove(this);
    delete layer_;
  }
}

void Layer::Add(Layer* child) {
  layer_->Add(child->layer());
}

void Layer::Remove(Layer* child) {
  layer_->Remove(child->layer());
}

void Layer::StackAtTop(Layer* child) {
  layer_->StackAtTop(child->layer());
}

void Layer::StackAbove(Layer* child, Layer* other) {
  layer_->StackAbove(child->layer(), other->layer());
}

void Layer::StackAtBottom(Layer* child) {
  layer_->StackAtBottom(child->layer());
}

void Layer::StackBelow(Layer* child, Layer* other) {
  layer_->StackBelow(child->layer(), other->layer());
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
  layer_->SetBounds(bounds);
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
  return true;
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

bool Layer::SchedulePaint(const gfx::Rect& invalid_rect) {
  layer_->SchedulePaint(invalid_rect);
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
}  // namespace ui
