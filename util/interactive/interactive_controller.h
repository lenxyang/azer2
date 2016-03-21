#pragma once

#include "base/observer_list.h"
#include "ui/gfx/geometry/point.h"
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
class InteractiveContext;
class InteractiveController;

class InteractiveControllerObserver {
 public:
  virtual void OnControllerPicked(InteractiveController* controller) {}
  virtual void OnControllerUnpicked(InteractiveController* controller) {}
};

class InteractiveController {
 public:
  explicit InteractiveController(InteractiveContext* ctx);
  virtual ~InteractiveController();
  InteractiveContext* context() { return context_;}
  const InteractiveContext* context() const { return context_;}

  virtual int32 GetPicking(const gfx::Point& pt) = 0;
  virtual void OnDragBegin(const gfx::Point& pt) {}
  virtual void OnDrag(const gfx::Point& pt) {};
  virtual void OnDragEnd(const gfx::Point& pt) {}
  void HitTest(const gfx::Point& pt);

  // render
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
 private:
  int32 state_;
  ObserverList<InteractiveControllerObserver> observer_list_;
  InteractiveContext* context_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveController);
};

}  // namespace azer
