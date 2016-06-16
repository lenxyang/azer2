#include "azer/util/interactive/graypoint.h"

#include "azer/render/camera.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {
GrayPoint::GrayPoint() 
    : scale_(1.0f, 1.0f, 1.0f) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_ = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  EntityDataPtr ptr(CreateArrow(effect_->vertex_desc()));
  arrow_ = new Entity(ptr.get());
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

GrayPoint::~GrayPoint() {
}
}  // namespace azer
