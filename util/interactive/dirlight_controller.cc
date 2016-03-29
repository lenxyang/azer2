#include "azer/util/interactive/dirlight_controller.h"

#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"

namespace azer {
DirLightControllerObj::DirLightControllerObj() {
  color_ = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  arrow_ = new Entity(CreateArrow(effect_->vertex_desc()));
}

void DirLightControllerObj::Update(const Camera* camera) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_->SetPV(camera->GetProjViewMatrix());
  effect_->SetWorld(world_);
  effect_->SetLightData(&env->light()->data(), 1);
}

void DirLightControllerObj::Render(Renderer* renderer) {
  ColorMaterialData mtrl;
  mtrl.diffuse = color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.alpha = 1.0f;
  color_effect_->SetMaterial(mtrl);
  renderer->BindEffect(effect_);
  arrow_->Draw(renderer);
}
}  // namespace azer
