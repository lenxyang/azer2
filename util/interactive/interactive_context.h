#pragma once

#include <vector>
#include "ui/event/event.h"
#include "nelf/render/event_listener.h"

namespace azer {

class InteractiveController;
class InteractiveContext : public nelf::EventListener {
 public:
  InteractiveContext();

  void Active(InteractiveController* controller);
  void AddController(InteractiveController* controller);
  void RemoveController(InteractiveController* controller);
  int GetIndexOf(InteractiveController* controller) const;
 protected:
  void OnMousePressed(const ui::MouseEvent& event) override;
  void OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  void OnMouseCaptureLost() override;
 private:
  void OnDragBegin(const ui::MouseEvent& event);
  void OnDragEnd(const ui::MouseEvent& event);
  int32 activated_;
  bool dragging_;
  std::vector<InteractiveController*> controllers_;
  DISALLOW_COPY_AND_ASSIGN(InteractiveContext);
};
}  // namespace azer
