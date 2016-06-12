#pragma once

#include <vector>
#include "ui/events/event.h"
#include "azer/ui/window.h"
#include "azer/ui/event_listener.h"
#include "azer/ui/render_subwindow.h"

namespace azer {
class Camera;
class FrameArgs;
class InteractiveController;
class Renderer;

class InteractiveContext : public azer::EventListener {
 public:
  InteractiveContext(RenderSubWindow* window, const Camera* camera);

  void AddController(InteractiveController* controller);
  void RemoveController(InteractiveController* controller);
  int GetIndexOf(InteractiveController* controller) const;
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  RenderSubWindow* window() { return window_;}
  const Camera* camera() const { return camera_;}
  Ray GetPickingRay(const gfx::Point& pt);
  const FrameArgs* args() const { return args_;}
 protected:
  // override from nelf::EventListener
  void OnKeyPressed(const ui::KeyEvent& event) override;
  void OnKeyReleased(const ui::KeyEvent& event) override;
  void OnMousePressed(const ui::MouseEvent& event) override;
  void OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  void OnMouseCaptureLost() override;

 private:
  void Activate(InteractiveController* controller);
  void Deactivate(InteractiveController* controller);
  friend class InteractiveController;
 private:
  void OnDragBegin(const ui::MouseEvent& event);
  void OnDrag(const ui::MouseEvent& event);
  void OnDragEnd(const ui::MouseEvent& event);
  int32_t activated_;
  bool draging_;
  ui::MouseEvent last_drag_event_;
  RenderSubWindow* window_;
  const Camera* camera_;
  const FrameArgs* args_;
  std::vector<InteractiveController*> controllers_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveContext);
};
}  // namespace azer
