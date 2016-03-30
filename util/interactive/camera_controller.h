#pragma once

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
  CameraController();
  ~CameraController() override;

  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const gfx::Point& pt) override;
  void OnDrag(const gfx::Point& pt) override;
  void OnDragEnd(const gfx::Point& pt) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(CameraController);
};
}   // namespace azer
