#pragma once

#include "azer/ui/win/window_delegate.h"

class MouseEventCounterDelegate : public azer::ui::WindowDelegate {
 public:
  MouseEventCounterDelegate() {
    ResetCounters();
  }

  void ResetCounters() {
    mouse_pressed_counter_ = 0;
    mouse_released_counter_ = 0;
    mouse_entered_counter_ = 0;
    mouse_exted_counter_ = 0;
    other_counter_++;
  }

  int mouse_pressed_counter() const { return mouse_pressed_counter_;}
  int mouse_released_counter() const { return mouse_released_counter_;}
  int mouse_entered_counter() const { return mouse_entered_counter_;}
  int mouse_exted_counter() const { return mouse_exted_counter_;}
  int other_counter() const { return other_counter_;}

  void OnMouseEvent(ui::MouseEvent* event) override {
    switch (event->type()) {
      case ET_MOUSE_PRESSED: mouse_pressed_counter_++; break;
      case ET_MOUSE_RELEASED: mouse_released_counter_++; break;
      case ET_MOUSE_ENTERED: mouse_entered_counter_++; break;
      case ET_MOUSE_EXITED: mouse_exited_counter_++; break;
      default: other_counter_++;
    }
  }
 private:
  int mouse_pressed_counter_;
  int mouse_released_counter_;
  int mouse_entered_counter_;
  int mouse_exted_counter_;
  int other_counter_;
  DISALLOW_COPY_AND_ASSIGN(MouseEventCounterDelegate);
};
