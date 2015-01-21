#pragma once

#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/api.h"

namespace azer {
namespace widget {
namespace testing {
class TestWidgetDelegate : public WidgetDelegate {
 public:
  TestWidgetDelegate();
  ~TestWidgetDelegate() override;

  void set_minimum_size(const gfx::Size& size) {minimum_size_ = size;}
  void set_maximum_size(const gfx::Size& size) {maximum_size_ = size;}
  void set_can_focus(bool can_focus) { can_focus_ = can_focus; }
  
  
  // override from WidgetDelegate
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  bool CanFocus() override;
  void OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  bool ShouldDescendIntoChildForEventHandling(Widget* child,
                                              const gfx::Point& location) override;
 private:
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;
  bool can_focus_;
  DISALLOW_COPY_AND_ASSIGN(TestWidgetDelegate);
};

class ColorWidgetDelegate : public TestWidgetDelegate {
 public:
  ColorWidgetDelegate(SkColor color) : color_(color) {}
  ~ColorWidgetDelegate() override {}

  void OnPaint(gfx::Canvas* canvas) override;
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(ColorWidgetDelegate);
};

// A simple EventHandler that keeps track of the number of key events that it's
// seen.
class WidgetEventCounter : public TestWidgetDelegate {
 public:
  WidgetEventCounter();
  ~WidgetEventCounter() override;

  int num_key_events() const { return num_key_events_; }
  int num_mouse_events() const { return num_mouse_events_; }
  int num_scroll_events() const { return num_scroll_events_; }
  int num_touch_events() const { return num_touch_events_; }
  int num_gesture_events() const { return num_gesture_events_; }

  void Reset();

  // EventHandler overrides:
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;

 private:
  // How many events have been received of each type?
  int num_key_events_;
  int num_mouse_events_;
  int num_scroll_events_;
  int num_touch_events_;
  int num_gesture_events_;

  std::string handler_name_;

  DISALLOW_COPY_AND_ASSIGN(WidgetEventCounter);
};
}  // namespace testing
}  // namespace widget
}  // namespace azer
