#include "azer/ui/aura/layer/layer.cc"

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
