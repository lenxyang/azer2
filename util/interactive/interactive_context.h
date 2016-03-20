#pragma once

#include <vector>
#include "ui/events/event.h"
#include "nelf/render/event_listener.h"

namespace azer {
class FrameArgs;
class InteractiveController;
class Renderer;

class InteractiveContext : public nelf::EventListener {
 public:
  InteractiveContext();

  void Activate(InteractiveController* controller);
  void AddController(InteractiveController* controller);
  void RemoveController(InteractiveController* controller);
  int GetIndexOf(InteractiveController* controller) const;
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);
 protected:
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
  std::vector<InteractiveController*> controllers_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveContext);
};
}  // namespace azer
