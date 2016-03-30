#pragma once

#include <vector>
#include "ui/events/event.h"
#include "nelf/window.h"
#include "nelf/render/event_listener.h"
#include "nelf/render/render_window.h"

namespace azer {
class Camera;
class FrameArgs;
class InteractiveController;
class Renderer;

class InteractiveContext : public nelf::EventListener {
 public:
  InteractiveContext(nelf::RenderWindow* window, const Camera* camera);

  void Activate(InteractiveController* controller);
  void Deactivate(InteractiveController* controller);
  void AddController(InteractiveController* controller);
  void RemoveController(InteractiveController* controller);
  int GetIndexOf(InteractiveController* controller) const;
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  nelf::RenderWindow* window() { return window_;}
  const Camera* camera() const { return camera_;}
  Ray GetPickingRay(const gfx::Point& pt);
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
  void OnDragBegin(const ui::MouseEvent& event);
  void OnDrag(const ui::MouseEvent& event);
  void OnDragEnd(const ui::MouseEvent& event);
  int32 activated_;
  bool draging_;
  nelf::RenderWindow* window_;
  const Camera* camera_;
  std::vector<InteractiveController*> controllers_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveContext);
};
}  // namespace azer
