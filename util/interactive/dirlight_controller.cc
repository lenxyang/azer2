#include "azer/util/interactive/dirlight_controller.h"

#include "base/logging.h"
#include "azer/render/camera.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {
DirLightControllerObj::DirLightControllerObj() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_ = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  arrow_ = new Entity(CreateArrow(effect_->vertex_desc()));
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void DirLightControllerObj::OnParamUpdate() {
  Matrix4 world_ = std::move(Translate(position_))
      * std::move(orientation_.ToMatrix())
      * std::move(Scale(scale_));
  effect_->SetWorld(world_);
  ColorMaterialData mtrl;
  mtrl.diffuse = color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.alpha = 1.0f;
  effect_->SetMaterial(mtrl);
}

void DirLightControllerObj::Update(const Camera* camera) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_->SetPV(camera->GetProjViewMatrix());
  effect_->SetLightData(&env->light()->data(), 1);
  OnParamUpdate();
}

void DirLightControllerObj::set_scale(const Vector3& v) { 
  scale_ = v;
  OnParamUpdate();
}

void DirLightControllerObj::set_position(const Vector3& pos) { 
  position_ = pos;
  OnParamUpdate();
}
void DirLightControllerObj::set_orientation(const Quaternion& q) { 
  orientation_ = q;
  OnParamUpdate();
}

void DirLightControllerObj::set_color(const Vector4& c) { 
  color_ = c; 
  OnParamUpdate();
}

void DirLightControllerObj::Render(Renderer* renderer) {
  renderer->BindEffect(effect_);
  arrow_->Draw(renderer);
}

// class DirLightController
DirLightController::DirLightController(Light* light, InteractiveContext* ctx)
    : InteractiveController(ctx),
      show_rotate_controller_(false),
      dragging_(false), 
      light_(light) {
  controller_.reset(new RotateController(ctx));
  controller_->AddRotateObserver(this);
  object_.reset(new DirLightControllerObj);
}

DirLightController::~DirLightController() {
  controller_->RemoveRotateObserver(this);
}

void DirLightController::set_position(const Vector3& pos) {
  object_->set_position(pos);
  controller_->set_position(pos);
}

const Vector3& DirLightController::position() const {
  return object_->position();
}

void DirLightController::show_rotate_controller(bool b) {
  show_rotate_controller_ = b;
}

void DirLightController::OnActive() {
  show_rotate_controller(true);
}

void DirLightController::OnDeactive() {
  show_rotate_controller(false);
}

int32 DirLightController::GetPicking(const gfx::Point& pt) {
  if (show_rotate_controller_) {
    return controller_->GetPicking(pt);
  } else {
    return 0;
  }
}

void DirLightController::OnDragBegin(const ui::MouseEvent& e) {
  dragging_ = true;
  if (show_rotate_controller_) {
    controller_->OnDragBegin(e);
  }
}

void DirLightController::OnDragging(const ui::MouseEvent& e) {
  if (show_rotate_controller_) {
    controller_->OnDragging(e);
  }
}

void DirLightController::OnDragEnd(const ui::MouseEvent& e) {
  if (show_rotate_controller_ || dragging_) {
    controller_->OnDragEnd(e);
  }

  dragging_ = false;
}

void DirLightController::UpdateFrame(const FrameArgs& args) {
  UpdateParam();
  controller_->set_state(state());
  controller_->UpdateFrame(args);
}

void DirLightController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer);
  if (show_rotate_controller_) {
    controller_->RenderFrame(renderer);
  }
}

void DirLightController::UpdateParam() {
  Quaternion quad;
  CalcSceneOrientForZDirection(light_->directional(), &quad);
  object_->set_color(light_->diffuse());
  object_->set_orientation(quad);
  object_->Update(context()->camera());
}

// observer funcs
void DirLightController::OnRotateBegin(RotateController* controller) {
}

void DirLightController::OnRotating(RotateController* controller) {
  Matrix4 mat = std::move(controller->orientation().ToMatrix());
  Vector3 dir = std::move(mat * Vector4(0.0f, 0.0f, 1.0f, 0.0));
  light_->set_directional(dir);
  UpdateParam();
}

void DirLightController::OnRotateEnd(RotateController* controller) {
  Matrix4 mat = std::move(controller->orientation().ToMatrix());
  Vector3 dir = std::move(mat * Vector4(0.0f, 0.0f, 1.0f, 0.0));
  light_->set_directional(dir);
  UpdateParam();
}
}  // namespace azer
