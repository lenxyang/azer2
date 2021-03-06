#include "azer/util/interactive/bounding_box.h"

#include "azer/effect/color_effect.h"
#include "azer/math/math.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/box.h"
#include "azer/util/interactive/env.h"

namespace azer {
BoundingBoxObj::BoundingBoxObj() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_ = Vector4(0.0f, 1.0f, 0.0f, 0.5f);
  color_effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  EntityDataPtr vd = CreateBox(color_effect_->vertex_desc());
  box_ = new Entity(vd.get());
  EntityDataPtr vbf = CreateBoxFrame(color_effect_->vertex_desc());
  boxframe_ = new Entity(vbf.get());
}

void BoundingBoxObj::SetCorner(const Vector3& min, const Vector3& max) {
  Vector3 position = (min + max) * 0.5f;
  Vector3 scale(max.x - min.x, max.y - min.y, max.z - min.z);
  world_ = std::move(Transform::Translate(position) * Transform::Scale(scale));
}

void BoundingBoxObj::Update(const Camera* camera) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_effect_->SetPV(camera->GetProjViewMatrix());
  color_effect_->SetWorld(world_);
  color_effect_->SetLightData(&env->light()->data(), 1);
}

void BoundingBoxObj::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  ScopedResetBlending scoped_blending(renderer);
  renderer->SetBlending(env->blending(), 0, 0xffffffff);
  
  ColorMaterialData mtrl;
  mtrl.diffuse = color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.alpha = 1.0f;
  color_effect_->SetMaterial(mtrl);
  renderer->BindEffect(color_effect_.get());
  boxframe_->Draw(renderer);

  mtrl.alpha = mtrl.diffuse.w;
  color_effect_->SetMaterial(mtrl);
  renderer->BindEffect(color_effect_.get());
  box_->Draw(renderer);
}
}  // namespace azer
