#pragma once

#include "ui/views/view.h"

namespace nelf {
class EventListener {
 public:
  virtual void OnMousePressed(const ui::MouseEvent& event) {}
  virtual void OnMouseDragged(const ui::MouseEvent& event) {}
  virtual void OnMouseReleased(const ui::MouseEvent& event) {}
  virtual void OnMouseMoved(const ui::MouseEvent& event) {}
  virtual void OnKeyPressed(const ui::KeyEvent& event) {}
  virtual void OnKeyReleased(const ui::KeyEvent& event) {}
  virtual void OnMouseWheel(const ui::MouseWheelEvent& event) {}
  virtual void OnMouseCaptureLost() {}
  virtual void OnLostFocus() {}
};

}  // namespace nelf
