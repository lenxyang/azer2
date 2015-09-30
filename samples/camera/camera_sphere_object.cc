#include "azer/samples/camera/camera_sphere_object.h"

namespace azer {
CameraSphereObject::CameraSphereObject(const Camera* camera) 
    : camera_(camera) {
  axes_.reset(new AxesFrames);
}

CameraSphereObject::~CameraSphereObject() {
}

void CameraSphereObject::Update() {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera_->GetProjViewMatrix() * world_);
}

void CameraSphereObject::Render(Renderer* renderer) {
  Update();
  axes_->Render(world_, pvw_, renderer);
}
}  // namespace azer
