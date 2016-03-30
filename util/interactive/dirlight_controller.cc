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
}

void DirLightControllerObj::set_scale(const Vector3& v) { 
  scale_ = v;
}

void DirLightControllerObj::set_position(const Vector3& pos) { 
  position_ = pos;
}
void DirLightControllerObj::set_orientation(const Quaternion& q) { 
  orientation_ = q;
}

void DirLightControllerObj::set_color(const Vector4& c) { 
  color_ = c; 
}

void DirLightControllerObj::Render(const Camera* camera, Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 world_ = std::move(Translate(position_))
      * std::move(orientation_.ToMatrix())
      * std::move(Scale(scale_));
  
  ColorMaterialData mtrl;
  mtrl.diffuse = color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.emission = mtrl.diffuse * 0.4f;
  mtrl.alpha = 1.0f;
  effect_->SetMaterial(mtrl);
  effect_->SetWorld(world_);
  effect_->SetPV(camera->GetProjViewMatrix());
  effect_->SetLightData(&env->light()->data(), 1);
  renderer->BindEffect(effect_);
  arrow_->Draw(renderer);
}

// class DirLightObject
DirLightObject::DirLightObject(Light* light) 
    : light_(light) {
  DCHECK_EQ(light->type(), kDirectionalLight);
  object_.reset(new DirLightControllerObj);
}

void DirLightObject::Render(const Camera* camera, Renderer* renderer) {
  Quaternion quad;
  CalcSceneOrientForZDirection(light_->directional(), &quad);
  object_->set_color(light_->diffuse());
  object_->set_position(position_);
  object_->set_orientation(quad);
  object_->Render(camera, renderer);
}

// class DirLightController
DirLightController::DirLightController(InteractiveContext* ctx)
    : InteractiveController(ctx),
      dragging_(false) {
  controller_.reset(new RotateController(ctx));
  controller_->AddRotateObserver(this);
}

DirLightController::~DirLightController() {
  controller_->RemoveRotateObserver(this);
}

void DirLightController::SetDirLightObj(DirLightObject* obj) { 
  lightobj_ = obj;
}

void DirLightController::OnActive() {}
void DirLightController::OnDeactive() {}

int32 DirLightController::GetPicking(const gfx::Point& pt) {
  if (lightobj_.get()) {
    return controller_->GetPicking(pt);
  } else {
    return 0;
  }
}

void DirLightController::OnDragBegin(const ui::MouseEvent& e) {
  dragging_ = true;
  controller_->OnDragBegin(e);
}

void DirLightController::OnDragging(const ui::MouseEvent& e) {
  controller_->OnDragging(e);
}

void DirLightController::OnDragEnd(const ui::MouseEvent& e) {
  controller_->OnDragEnd(e);
  dragging_ = false;
}

void DirLightController::UpdateFrame(const FrameArgs& args) {
  if (lightobj_.get()) {
    controller_->set_state(state());
    controller_->Update(args);
  }
}

void DirLightController::RenderFrame(Renderer* renderer) {
  if (lightobj_.get()) {
    controller_->set_position(lightobj_->position());
    controller_->Render(renderer);
  }
}

// observer funcs
void DirLightController::OnRotateBegin(RotateController* controller) {
}

void DirLightController::OnRotating(RotateController* controller) {
  if (lightobj_.get()) {
    Quaternion quad;
    CalcSceneOrientForZDirection(light()->directional(), &quad);
    Quaternion q = controller->orientation() * quad;
    Matrix4 mat = std::move(q.ToMatrix());
    Vector3 dir = std::move(mat * Vector4(0.0f, 0.0f, 1.0f, 0.0));
    light()->set_directional(dir);
  }
}

void DirLightController::OnRotateEnd(RotateController* controller) {
  if (lightobj_.get()) {
    Quaternion quad;
    CalcSceneOrientForZDirection(light()->directional(), &quad);
    Quaternion q = controller->orientation() * quad;
    Matrix4 mat = std::move(q.ToMatrix());
    Vector3 dir = std::move(mat * Vector4(0.0f, 0.0f, 1.0f, 0.0));
    light()->set_directional(dir);
  }
}
}  // namespace azer
