#include "azer/util/interactive/camera_controller.h"

#include "azer/render/camera.h"
#include "azer/render/frame_args.h"
#include "azer/util/interactive/interactive_context.h"

namespace azer {
CameraController::CameraController(InteractiveContext* ctx)
    : InteractiveController(ctx) {
  move_speed_ = 32.0f;
  rotate_speed_ = Radians(3.14f);
  orientation_dragging_ = false;
  ResetState();
}

CameraController::~CameraController() {}

void CameraController::ResetState() {
  posx_ = 0;
  posz_ = 0;
  posy_ = 0;
  negx_ = 0;
  negz_ = 0;
  negy_ = 0;
}

bool CameraController::OnKeyPressed(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_W) {
    posz_ = 1;
    return true;
  } else if (event.key_code() == ui::VKEY_S) {
    negz_ = 1;
    return true;
  } else if (event.key_code() == ui::VKEY_A) {
    negx_ = 1;
    return true;
  } else if (event.key_code() == ui::VKEY_D) {
    posx_ = 1;
    return true;
  } else if (event.key_code() == ui::VKEY_F) {
    posy_ = 1;
    return true;
  } else if (event.key_code() == ui::VKEY_R) {
    negy_ = 1;
    return true;
  } else {
    return false;
  }
}

bool CameraController::OnKeyReleased(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_W) {
    posz_ = 0;
    return true;
  } else if (event.key_code() == ui::VKEY_S) {
    negz_ = 0;
    return true;
  } else if (event.key_code() == ui::VKEY_A) {
    negx_ = 0;
    return true;
  } else if (event.key_code() == ui::VKEY_D) {
    posx_ = 0;
    return true;
  } else if (event.key_code() == ui::VKEY_F) {
    posy_ = 0;
    return true;
  } else if (event.key_code() == ui::VKEY_R) {
    negy_ = 0;
    return true;
  } else {
    return false;
  }
}

int32_t CameraController::GetPicking(const gfx::Point& pt) {
  return 1;
}

void CameraController::OnDragBegin(const ui::MouseEvent& event) {
  location_ = event.location();
  if (event.IsLeftMouseButton() && event.GetClickCount() == 1) {
    origin_orient_ = camera()->holder().orientation();
    orientation_dragging_ = true;
  }
}

void CameraController::OnDragging(const ui::MouseEvent& event) {
  if (orientation_dragging_) {
    RotateCamera(location_, event.location());
  }
}

void CameraController::OnDragEnd(const ui::MouseEvent& event) {
  if (orientation_dragging_) {
    RotateCamera(location_, event.location());
    orientation_dragging_ = false;
  }
}

void CameraController::RotateCamera(const gfx::Point& prev, const gfx::Point& cur) {
  camera()->SetOrientation(origin_orient_);
  Degree to_yaw = Degree(cur.x() - prev.x()) * 0.2f;
  // holder->yaw(to_yaw);
  camera()->rotate(azer::Vector3(0.0f, 1.0f, 0.0f), to_yaw);
  Degree to_pitch = Degree(cur.y() - prev.y()) * 0.1;
  camera()->pitch(to_pitch);
}

void CameraController::UpdateFrame(const azer::FrameArgs& args) {
  DCHECK(camera());
  float unit = args.delta().InSecondsF() * move_speed_;
  camera()->strafe((posx_ - negx_) * unit);
  camera()->walk((posz_ - negz_) * unit);
  camera()->fly((posy_ - negy_)* unit);
}

const Camera* CameraController::camera() const {
  return context()->camera();
}

Camera* CameraController::camera() {
  return const_cast<Camera*>(
      const_cast<const CameraController*>(this)->camera());
}

void CameraController::OnActive() {
}

void CameraController::OnDeactive() {
  ResetState();
}
}  // namespace azer
