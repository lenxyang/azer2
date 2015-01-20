#pragma once

#include "azer/base/export.h"

namespace azer {
namespace views {

class View;

class AZER_EXPORT ViewEventHandler {
 public:
  virtual bool OnMousePressed(View* view, const ui::MouseEvent& event) = 0;
  virtual bool OnMouseReleased(View* view, const ui::MouseEvent& event) = 0;
  virtual bool OnMouseMoved(View* view, const ui::MouseEvent& event) = 0;
  virtual bool OnMouseEntered(View* view, const ui::MouseEvent& event) = 0;
  virtual bool OnMouseExited(View* view, const ui::MouseEvent& event) = 0;
  virtual bool OnMouseWheel(View* view, const ui::MouseWheelEvent& event) = 0;

  virtual bool OnKeyPressed(View* view, const ui::KeyEvent& e) { return false;}
  virtual bool OnKeyReleased(View* view, const ui::KeyEvent& e) { return false;}
 protected:
  ViewEventHandler() {}
  virtual ~ViewEventHandler() {}
  DISALLOW_COPY_AND_ASSIGN(ViewEventHandler);
};
}  // namespace views
}  // namespace azer
