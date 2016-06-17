#include "azer/util/interactive/graypoint.h"

#include "azer/render/camera.h"
#include "azer/effect/effectlib.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {
GrayPoint::GrayPoint() 
    : scale_(1.0f, 1.0f, 1.0f) {
  color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  effect_ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  EntityDataPtr ptr(CreateArrow(effect_->vertex_desc()));
  arrow_ = new Entity(ptr.get());
  scale_ = Vector3(1.0f, 1.0f, 1.0f);

  
}

GrayPoint::~GrayPoint() {
}

void GrayPoint::Render(const Camera& camera, Renderer* renderer) {
  Matrix4 world_ = std::move(Translate(position_)) * std::move(Scale(scale_));
  
}
}  // namespace azer
