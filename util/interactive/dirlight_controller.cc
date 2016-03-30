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

void DirLightControllerObj::Update(const Camera* camera) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 world_ = std::move(Translate(position_))
      * std::move(orientation_.ToMatrix())
      * std::move(Scale(scale_));
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
  effect_->SetMaterial(mtrl);
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

int32 DirLightController::GetPicking(const gfx::Point& pt) {
  if (show_rotate_controller_) {
    return controller_->GetPicking(pt);
  } else {
    return 0;
  }
}

void DirLightController::OnDragBegin(const gfx::Point& pt) {
  dragging_ = true;
  if (show_rotate_controller_) {
    controller_->OnDragBegin(pt);
  }
}

void DirLightController::OnDrag(const gfx::Point& pt) {
  if (show_rotate_controller_) {
    controller_->OnDrag(pt);
  }
}

void DirLightController::OnDragEnd(const gfx::Point& pt) {
  if (show_rotate_controller_ || dragging_) {
    controller_->OnDrag(pt);
  }

  dragging_ = false;
}

void DirLightController::UpdateFrame(const FrameArgs& args) {
  Quaternion quad;
  CalcSceneOrientForZDirection(light_->directional(), &quad);
  object_->set_color(light_->diffuse());
  object_->set_orientation(quad);
  object_->Update(context()->camera());
  controller_->set_state(state());
  controller_->UpdateFrame(args);
}

void DirLightController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer);
  if (show_rotate_controller_) {
    controller_->RenderFrame(renderer);
  }
}

// observer funcs
void DirLightController::OnRotateBegin(RotateController* controller) {
}

void DirLightController::OnRotating(RotateController* controller) {
}

void DirLightController::OnRotateEnd(RotateController* controller) {
}
}  // namespace azer
