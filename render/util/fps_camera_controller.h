#pragma once

#include "azer/render/render.h"
#include "azer/render/frame_args.h"
#include "nelf/render/event_listener.h"

namespace azer {

class TransformHolderRetore {
 public:
  explicit TransformHolderRetore(TransformHolder* obj);
  void store();
  void restore();
  void reset() { 
    position_ = Vector3();
    orientation_ = Quaternion();
    stored_ = false;
  }
 private:
  Vector3 position_;
  Quaternion orientation_;
  TransformHolder* object_;
  bool stored_;
  DISALLOW_COPY_AND_ASSIGN(TransformHolderRetore);
};

class FPSCameraController : public nelf::EventListener {
 public:
  FPSCameraController(Camera* camera);

  void Update(const azer::FrameArgs& args);

  void OnKeyPressed(const ui::KeyEvent& event) override;
  void OnKeyReleased(const ui::KeyEvent& event) override;
  void OnMousePressed(const ui::MouseEvent& event) override;
  void OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;;
  void OnMouseCaptureLost() override;
  void OnLostFocus() override;
 private:
  void ResetState();
  void RotateCamera(const gfx::Point& prev, const gfx::Point& cur);

  bool orientation_dragging_;
  azer::Camera* camera_;
  bool posx_, posy_, posz_;
  bool negx_, negy_, negz_;
  gfx::Point location_;
  TransformHolderRetore storer_;
  DISALLOW_COPY_AND_ASSIGN(FPSCameraController);
};

}  // namespace azer
