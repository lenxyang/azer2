#pragma once

#include "ui/gfx/geometry/point.h"
#include "azer/math/quaternion.h"
#include "azer/math/radius.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {

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

  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const gfx::Point& pt) override;
  void OnDrag(const gfx::Point& pt) override;
  void OnDragEnd(const gfx::Point& pt) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  bool OnMouseReleased(const ui::MouseEvent& event) override;

  void set_rotate_speed(Radius rad) { rotate_speed_ = rad;}
  void set_speed(float speed) { move_speed_ = speed;}
 private:
  ResetState();
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
