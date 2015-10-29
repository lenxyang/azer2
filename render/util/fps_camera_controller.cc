#include "azer/render/util/fps_camera_controller.h"

namespace azer {
// class TransformHolderRetore
TransformHolderRetore::TransformHolderRetore(TransformHolder* obj)
    : object_(obj),
      stored_(false) {
}

void TransformHolderRetore::store() {
  stored_ = true;
  position_ = object_->position();
  orientation_ = object_->orientation();
}

void TransformHolderRetore::restore() {
  DCHECK(stored_);
  object_->SetPosition(position_);
  object_->set_orientation(orientation_);
}

// class FPSCameraController
FPSCameraController::FPSCameraController(Camera* camera) 
    : camera_(camera),
      posx_(0),
      posz_(0),
      posy_(0),
      negx_(0),
      negz_(0),
      negy_(0),
      storer_(camera->GetTransformHolder()) {
  orientation_dragging_ = false;
}

void FPSCameraController::ResetState() {
  posx_ = 0;
  posz_ = 0;
  posy_ = 0;
  negx_ = 0;
  negz_ = 0;
  negy_ = 0;
}

void FPSCameraController::OnKeyPressed(const ui::KeyEvent& event) {
  Camera* camera = camera_;
  if (event.key_code() == ui::VKEY_W) {
    posz_ = 1;
  } else if (event.key_code() == ui::VKEY_S) {
    negz_ = 1;
  } else if (event.key_code() == ui::VKEY_A) {
    negx_ = 1;
  } else if (event.key_code() == ui::VKEY_D) {
    posx_ = 1;
  } else if (event.key_code() == ui::VKEY_F) {
    posy_ = 1;
  } else if (event.key_code() == ui::VKEY_R) {
    negy_ = 1;
  }
}

void FPSCameraController::OnKeyReleased(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_W) {
    posz_ = 0;
  } else if (event.key_code() == ui::VKEY_S) {
    negz_ = 0;
  } else if (event.key_code() == ui::VKEY_A) {
    negx_ = 0;
  } else if (event.key_code() == ui::VKEY_D) {
    posx_ = 0;
  } else if (event.key_code() == ui::VKEY_F) {
    posy_ = 0;
  } else if (event.key_code() == ui::VKEY_R) {
    negy_ = 0;
  }
}

void FPSCameraController::OnMouseCaptureLost() {
  if (orientation_dragging_) {
    storer_.reset();
    orientation_dragging_ = false;
  }

  ResetState();
}

void FPSCameraController::OnLostFocus() {
  if (orientation_dragging_) {
    storer_.reset();
    orientation_dragging_ = false;
  }

  ResetState();
}

void FPSCameraController::Update(const azer::FrameArgs& args) {
  float unit = args.delta().InSecondsF() * 32.0f;
  TransformHolder* holder = camera_->GetTransformHolder();
  holder->strafe((posx_ - negx_) * unit);
  holder->walk((posz_ - negz_) * unit);
  holder->fly((posy_ - negy_)* unit);
  camera_->Update();
}

void FPSCameraController::OnMousePressed(const ui::MouseEvent& event) {
  location_ = event.location();
  if (event.IsLeftMouseButton() && event.GetClickCount() == 1) {
    orientation_dragging_ = true;
    storer_.store();
  }
}
void FPSCameraController::OnMouseDragged(const ui::MouseEvent& event) {
  if (orientation_dragging_) {
    RotateCamera(location_, event.location());
  }
}

void FPSCameraController::OnMouseReleased(const ui::MouseEvent& event) {
  if (orientation_dragging_) {
    RotateCamera(location_, event.location());
    storer_.reset();
    orientation_dragging_ = false;
  }
}
void FPSCameraController::RotateCamera(const gfx::Point& prev, 
                                       const gfx::Point& cur) {
  using azer::Degree;
  storer_.restore();
  TransformHolder* holder = camera_->GetTransformHolder();
  Degree to_yaw = Degree(cur.x() - prev.x()) * 0.1;
  // holder->yaw(to_yaw);
  holder->rotate(azer::Vector3(0.0f, 1.0f, 0.0f), to_yaw);
  Degree to_pitch = Degree(cur.y() - prev.y()) * 0.1;
  holder->pitch(to_pitch);
}

}  // namespace azer
