#include "azer/util/interactive/dirlight_controller.h"

#include "base/logging.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/interactive_context.h"
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

// class DirLightController
DirLightController::DirLightController(Light* light, InteractiveContext* ctx)
    : LightController(ctx),
      light_(light) {
  controller_.reset(new RotateController(ctx));
  controller_->AddRotateObserver(this);
  object_.reset(new DirLightControllerObj);
}

DirLightController::~DirLightController() {
  controller_->RemoteRotateObserver(this);
}

void DirLightController::set_position(const Vector3& pos) {
  object_->set_position(pos);
}

const DirLightController::Vector3& position() const {
  return object_->position();
}

int32 DirLightController::GetPicking(const gfx::Point& pt) {
  return 0;
}

void DirLightController::OnDragBegin(const gfx::Point& pt) {
}

void DirLightController::OnDrag(const gfx::Point& pt) {
}

void DirLightController::OnDragEnd(const gfx::Point& pt) {
}

void DirLightController::UpdateFrame(const FrameArgs& args) {
  object_->set_color(light_->diffuse());
  object_->set_orientation(light);
  object_->Update(args);
}

void DirLightController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer, context()->camera());
}

// observer funcs
void DirLightController::OnRotateBegin(RotateController* controller) {
}

void DirLightController::OnTranslating(RotateController* controller) {
}

void DirLightController::OnRotateEnd(RotateController* controller) {
}
}  // namespace azer
