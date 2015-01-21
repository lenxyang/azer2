#include "azer/ui/widget/api.h"

#include "ui/events/keycodes/keyboard_codes.h"
#include "azer/ui/widget/tests/event_generator.h"
#include "azer/ui/widget/tests/test_base.h"
#include "azer/ui/widget/tests/test_widget_delegate.h"

namespace azer {
namespace widget {
namespace testing {

// Used in verifying mouse capture.
namespace {
class CaptureWidgetDelegateImpl : public TestWidgetDelegate {
 public:
  CaptureWidgetDelegateImpl() {
    ResetCounts();
  }

  void ResetCounts() {
    capture_changed_event_count_ = 0;
    capture_lost_count_ = 0;
    mouse_event_count_ = 0;
    touch_event_count_ = 0;
    gesture_event_count_ = 0;
  }

  int capture_changed_event_count() const {
    return capture_changed_event_count_;
  }
  int capture_lost_count() const { return capture_lost_count_; }
  int mouse_event_count() const { return mouse_event_count_; }
  int touch_event_count() const { return touch_event_count_; }
  int gesture_event_count() const { return gesture_event_count_;}
  void OnMouseEvent(ui::MouseEvent* event) override {
    if (event->type() == ui::ET_MOUSE_CAPTURE_CHANGED)
      capture_changed_event_count_++;
    mouse_event_count_++;
  }
  void OnTouchEvent(ui::TouchEvent* event) override { touch_event_count_++; }
  void OnGestureEvent(ui::GestureEvent* event) override {
    gesture_event_count_++;
  }
  void OnCaptureLost() override { capture_lost_count_++; }

 private:
  int capture_changed_event_count_;
  int capture_lost_count_;
  int mouse_event_count_;
  int touch_event_count_;
  int gesture_event_count_;

  DISALLOW_COPY_AND_ASSIGN(CaptureWidgetDelegateImpl);
};
}  // namespace
  

class WidgetTest : public WidgetTestBase {
 public:
  WidgetTest() {}
  ~WidgetTest() override {}

  void SetUp() override {
    WidgetTestBase::SetUp();
  }
  void TearDown() override {
    WidgetTestBase::TearDown();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(WidgetTest);
};

TEST_F(WidgetTest, Basic) {
}

TEST_F(WidgetTest, Destroy) {
  gfx::Rect bounds(100, 100, 300, 300);
  ColorWidgetDelegate delegate(SK_ColorRED);
  scoped_ptr<Widget> widget(CreateWidget(bounds, &delegate));
  widget->SetName("widget1");

  host_->Show();
  RunFirstFrame();
}

TEST_F(WidgetTest, MouseMove) {
}

TEST_F(WidgetTest, Capture) {
  gfx::Rect bounds(0, 0, 20, 20);
  CaptureWidgetDelegateImpl delegate;
  scoped_ptr<Widget> widget(CreateWidget(bounds, &delegate));
  widget->SetName("widget1");
  host_->Show();

  EXPECT_FALSE(widget->HasCapture());
  delegate.ResetCounts();

  // Do a capture.
  widget->SetCapture();
  EXPECT_TRUE(widget->HasCapture());
  EXPECT_EQ(0, delegate.capture_lost_count());
  EXPECT_EQ(0, delegate.capture_changed_event_count());
  EventGenerator generator(root(), gfx::Point(50, 50));
  generator.PressLeftButton();
  EXPECT_EQ(1, delegate.mouse_event_count());
  generator.ReleaseLeftButton();

  EXPECT_EQ(2, delegate.mouse_event_count());
  delegate.ResetCounts();

  RunFirstFrame();
}

}  // namespace testing
}  // namespace widget
}  // namespace azer
