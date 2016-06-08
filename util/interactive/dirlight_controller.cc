#include "azer/util/interactive/dirlight_controller.h"

#include "base/logging.h"
#include "azer/render/camera.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/util/interactive/arrow_object.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {

// class DirLightObject
DirLightObject::DirLightObject(Light* light) 
    : light_(light) {
  DCHECK_EQ(light->type(), kDirectionalLight);
  object_.reset(new ArrowObject);
}

void DirLightObject::Render(const Camera& camera, Renderer* renderer) {
  object_->set_color(light_->diffuse());
  object_->set_position(position_);
  object_->set_directional(light_->directional());
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

int32_t DirLightController::GetPicking(const gfx::Point& pt) {
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
    Vector3 dir = RotateLightDir(light()->directional(), controller->orientation());
    light()->set_directional(dir);
  }
}

void DirLightController::OnRotateEnd(RotateController* controller) {
  if (lightobj_.get()) {
    Vector3 dir = RotateLightDir(light()->directional(), controller->orientation());
    light()->set_directional(dir);
  }
}
}  // namespace azer
