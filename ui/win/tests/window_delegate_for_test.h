#pragma once

#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/ui/win/window_delegate.h"

namespace azer {
namespace win {

class WindowDelegateForTest : public WindowDelegate {
 public:
  virtual gfx::Size GetMinimumSize() const { return gfx::Size(1, 1);}
  virtual gfx::Size GteMaximumSize() const { return gfx::Size(10000, 10000);}

  virtual bool CanFocus() { return true;}
  virtual bool OnCaptureLost() {return true;}
  virtual void OnPaint(gfx::Canvas* canvas) {}
  virtual bool ShouldDescendIntoChildForEventHandling(
      Window* child, const gfx::Point& location) {}
};

class MouseEventCounterDelegate : public WindowDelegate {
 public:
  MouseEventCounterDelegate() {
    ResetCounters();
  }

  void ResetCounters() {
    mouse_pressed_counter_ = 0;
    mouse_released_counter_ = 0;
    mouse_entered_counter_ = 0;
    mouse_exited_counter_ = 0;
    other_counter_++;
  }

  int mouse_pressed_counter() const { return mouse_pressed_counter_;}
  int mouse_released_counter() const { return mouse_released_counter_;}
  int mouse_entered_counter() const { return mouse_entered_counter_;}
  int mouse_exted_counter() const { return mouse_exited_counter_;}
  int other_counter() const { return other_counter_;}

  void OnMouseEvent(::ui::MouseEvent* event) override {
    switch (event->type()) {
      case ::ui::ET_MOUSE_PRESSED: mouse_pressed_counter_++; break;
      case ::ui::ET_MOUSE_RELEASED: mouse_released_counter_++; break;
      case ::ui::ET_MOUSE_ENTERED: mouse_entered_counter_++; break;
      case ::ui::ET_MOUSE_EXITED: mouse_exited_counter_++; break;
      default: other_counter_++;
    }
  }
 private:
  int mouse_pressed_counter_;
  int mouse_released_counter_;
  int mouse_entered_counter_;
  int mouse_exited_counter_;
  int other_counter_;
  DISALLOW_COPY_AND_ASSIGN(MouseEventCounterDelegate);
};

}  // namespace wing
}  // namespace azer
