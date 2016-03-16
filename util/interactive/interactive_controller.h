#pragma once

#include "base/observer_list.h"
#include "azer/base/export.h"
#include "azer/math/ray.h"
#include "azer/math/vector3.h"

namespace azer {

enum {
  kInteractStandard = 0,
  kInteractScaleX,
  kInteractScaleY,
  kInteractScaleZ,
  kInteractScaleXY,
  kInteractScaleYZ,
  kInteractScaleZX,
  kInteractScaleXYZ,
  kInteractTransformX,
  kInteractTransformY,
  kInteractTransformZ,
  kInteractRotateX,
  kInteractRotateY,
  kInteractRotateZ,
  kInteractCustomer = 0x0000f000,
};


class FrameArgs;
class Renderer;
class InteractiveController;

class InteractiveControllerObserver {
 public:
  virtual void OnControllerPicked(InteractiveController* controller) {}
  virtual void OnControllerUnpicked(InteractiveController* controller) {}
  virtual void OnControllerChanged(InteractiveController* controller) {}
};

class InteractiveController {
 public:
  InteractiveController();
  virtual ~InteractiveController();
  virtual int32 GetPicking(const Ray& ray, Vector3* pos) = 0;
  virtual void UpdateFrame(const FrameArgs& args) = 0;
  virtual void RenderFrame(Renderer* renderer) = 0;

  void set_state(int32 state);
  int32 state() const { return state_;}
  void AddObserver(InteractiveControllerObserver* observer);
  void Removebserver(InteractiveControllerObserver* observer);
  bool HasObserver(InteractiveControllerObserver* observer);
 protected:
  void OnControllerPicked();
  void OnControllerUnpicked();
  void OnControllerChanged();
 private:
  int32 state_;
  ObserverList<InteractiveControllerObserver> observer_list_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveController);
};

}  // namespace azer
