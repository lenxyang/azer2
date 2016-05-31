#pragma once

#include "base/observer_list.h"
#include "ui/gfx/geometry/point.h"
#include "ui/events/event.h"
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

  void Activate();
  void Deactivate();
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  virtual void OnActive() {}
  virtual void OnDeactive() {}
  virtual int32_t GetPicking(const gfx::Point& pt) = 0;
  virtual void OnDragBegin(const ui::MouseEvent& e) {}
  virtual void OnDragging(const ui::MouseEvent& e) {};
  virtual void OnDragEnd(const ui::MouseEvent& e) {}
  virtual bool OnKeyPressed(const ui::KeyEvent& event) { return false;}
  virtual bool OnKeyReleased(const ui::KeyEvent& event) { return false;}
  void HitTest(const gfx::Point& pt);

  // render
  virtual void UpdateFrame(const FrameArgs& args) = 0;
  virtual void RenderFrame(Renderer* renderer) = 0;

  void set_state(int32_t state);
  int32_t state() const { return state_;}
  void AddObserver(InteractiveControllerObserver* observer);
  void Removebserver(InteractiveControllerObserver* observer);
  bool HasObserver(InteractiveControllerObserver* observer);
 protected:
  void OnControllerPicked();
  void OnControllerUnpicked();
 private:
  int32_t state_;
  ObserverList<InteractiveControllerObserver> observer_list_;
  InteractiveContext* context_;
  bool activate_frame_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveController);
};

}  // namespace azer
