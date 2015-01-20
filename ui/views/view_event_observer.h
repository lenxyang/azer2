#pragma once

#include "azer/base/export.h"
#include "ui/events/event.h"

namespace azer {
namespace views {

class View;

class AZER_EXPORT ViewEventObserver {
 public:
  virtual void OnMousePressed(View* view, const ui::MouseEvent& e) {}
  virtual void OnMouseReleased(View* view, const ui::MouseEvent& e) {}
  virtual void OnMouseMoved(View* view, const ui::MouseEvent& e) {}
  virtual void OnMouseEntered(View* view, const ui::MouseEvent& e) {}
  virtual void OnMouseExited(View* view, const ui::MouseEvent& e) {}
  virtual void OnMouseWheel(View* view, const ui::MouseWheelEvent& e) {}
  virtual void OnKeyPressed(View* view, const ui::KeyEvent& e) {}
  virtual void OnKeyReleased(View* view, const ui::KeyEvent& e) {}
 protected:
  ViewEventObserver() {}
  virtual ~ViewEventObserver() {}
  DISALLOW_COPY_AND_ASSIGN(ViewEventObserver);
};
}  // namespace views
}  // namespace azer
