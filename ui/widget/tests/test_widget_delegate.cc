#include "azer/ui/widget/tests/test_widget_delegate.h"

namespace azer {
namespace widget {
namespace testing {

TestWidgetDelegate::TestWidgetDelegate()
    : can_focus_(true) {
}

TestWidgetDelegate::~TestWidgetDelegate() {
}

gfx::Size TestWidgetDelegate::GetMinimumSize() const {
  return minimum_size_;
}

gfx::Size TestWidgetDelegate::GetMaximumSize() const {
  return maximum_size_;
}

bool TestWidgetDelegate::CanFocus() {
  return can_focus_;
}

void TestWidgetDelegate::OnCaptureLost() {
}

void TestWidgetDelegate::OnPaint(gfx::Canvas* canvas) {
}

bool TestWidgetDelegate::ShouldDescendIntoChildForEventHandling( 
    Widget* child, const gfx::Point& location) {
  return true;
}

void ColorWidgetDelegate::OnPaint(gfx::Canvas* canvas) {
  canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
}
   
WidgetEventCounter::WidgetEventCounter()
    : num_key_events_(0),
      num_mouse_events_(0),
      num_scroll_events_(0),
      num_touch_events_(0),
      num_gesture_events_(0) {
}

WidgetEventCounter::~WidgetEventCounter() {}

void WidgetEventCounter::Reset() {
  num_key_events_ = 0;
  num_mouse_events_ = 0;
  num_scroll_events_ = 0;
  num_touch_events_ = 0;
  num_gesture_events_ = 0;
}

void WidgetEventCounter::OnKeyEvent(ui::KeyEvent* event) {
  num_key_events_++;
  event->SetHandled();
}

void WidgetEventCounter::OnMouseEvent(ui::MouseEvent* event) {
  num_mouse_events_++;
}

void WidgetEventCounter::OnScrollEvent(ui::ScrollEvent* event) {
  num_scroll_events_++;
}

void WidgetEventCounter::OnTouchEvent(ui::TouchEvent* event) {
  num_touch_events_++;
}

void WidgetEventCounter::OnGestureEvent(ui::GestureEvent* event) {
  num_gesture_events_++;
}

}  // namespace testing
}  // namespace widget
}  // namespace azer
