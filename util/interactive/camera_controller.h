#pragma once

#include "ui/gfx/geometry/point.h"
#include "azer/math/quaternion.h"
#include "azer/math/degree.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class Camera;
class CameraControllerObserver {
 public:
  CameraControllerObserver();
  ~CameraControllerObserver();
 private:
  DISALLOW_COPY_AND_ASSIGN(CameraControllerObserver);
};

class CameraController : public InteractiveController {
 public:
  explicit CameraController(InteractiveContext* ctx);
  ~CameraController() override;

  void OnActive() override;
  void OnDeactive() override;
  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const ui::MouseEvent& e) override;
  void OnDragging(const ui::MouseEvent& e) override;
  void OnDragEnd(const ui::MouseEvent& e) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override {}

  void set_rotate_speed(Radians rad) { rotate_speed_ = rad;}
  void set_speed(float speed) { move_speed_ = speed;}
  const Camera* camera() const;
  Camera* camera();
 private:
  void ResetState();
  void RotateCamera(const gfx::Point& prev, const gfx::Point& cur);
  Quaternion origin_orient_;
  bool orientation_dragging_;
  bool posx_, posy_, posz_;
  bool negx_, negy_, negz_;
  gfx::Point location_;
  float move_speed_;
  Radians rotate_speed_;
  DISALLOW_COPY_AND_ASSIGN(CameraController);
};
}   // namespace azer
