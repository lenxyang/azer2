#include "azer/util/interactive/arrow_object.h"

#include "azer/render/camera.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {
ArrowObject::ArrowObject() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_ = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  EntityDataPtr ptr(CreateArrow(effect_->vertex_desc()));
  
arrow_ = new Entity(ptr.get());
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void ArrowObject::OnParamUpdate() {
}

void ArrowObject::set_directional(const Vector3& dir) {
  Quaternion quad;
  CalcSceneOrientForZDirection(dir, &quad);
  set_orientation(quad);
}

void ArrowObject::set_scale(const Vector3& v) { 
  scale_ = v;
}

void ArrowObject::set_position(const Vector3& pos) { 
  position_ = pos;
}
void ArrowObject::set_orientation(const Quaternion& q) { 
  orientation_ = q;
}

void ArrowObject::set_color(const Vector4& c) { 
  color_ = c; 
}

void ArrowObject::Render(const Camera& camera, Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 world_ = std::move(Transform::Translate(position_))
      * std::move(orientation_.ToMatrix())
      * std::move(Transform::Scale(scale_));
  
  ColorMaterialData mtrl;
  mtrl.diffuse = color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.emission = mtrl.diffuse * 0.4f;
  mtrl.alpha = 1.0f;
  effect_->SetMaterial(mtrl);
  effect_->SetWorld(world_);
  effect_->SetPV(camera.GetProjViewMatrix());
  effect_->SetLightData(&env->light()->data(), 1);
  renderer->BindEffect(effect_.get());
  arrow_->Draw(renderer);
}
}  // namespace azer
