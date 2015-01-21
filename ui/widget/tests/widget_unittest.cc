#include "azer/ui/widget/api.h"

#include "ui/events/keycodes/keyboard_codes.h"
#include "azer/ui/widget/widget_event_dispatcher.h"
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
  scoped_ptr<Widget> widget(CreateWidget(&delegate, bounds));
  widget->SetName("widget1");

  host_->Show();
  RunFirstFrame();
}

TEST_F(WidgetTest, MouseMove) {
}

class MouseEnterExitWidgetDelegate : public TestWidgetDelegate {
 public:
  MouseEnterExitWidgetDelegate() : entered_(false), exited_(false) {}

  void OnMouseEvent(ui::MouseEvent* event) override {
    switch (event->type()) {
      case ui::ET_MOUSE_ENTERED:
        EXPECT_TRUE(event->flags() & ui::EF_IS_SYNTHESIZED);
        entered_ = true;
        break;
      case ui::ET_MOUSE_EXITED:
        EXPECT_TRUE(event->flags() & ui::EF_IS_SYNTHESIZED);
        exited_ = true;
        break;
      default:
        break;
    }
  }

  bool entered() const { return entered_; }
  bool exited() const { return exited_; }

  // Clear the entered / exited states.
  void ResetExpectations() {
    entered_ = false;
    exited_ = false;
  }

 private:
  bool entered_;
  bool exited_;

  DISALLOW_COPY_AND_ASSIGN(MouseEnterExitWidgetDelegate);
};

// Verifies that the WidgetDelegate receives MouseExit and MouseEnter events for
// mouse transitions from window to window.
TEST_F(WidgetTest, MouseEnterExit) {
  MouseEnterExitWidgetDelegate d1;
  scoped_ptr<Widget> w1(CreateWidget(&d1, 1, gfx::Rect(10, 10, 50, 50),
                                     root_widget()));
  MouseEnterExitWidgetDelegate d2;
  scoped_ptr<Widget> w2(CreateWidget(&d2, 2, gfx::Rect(70, 70, 50, 50),
                                     root_widget()));

  EventGenerator generator(root_widget());
  generator.MoveMouseToCenterOf(w1.get());
  EXPECT_TRUE(d1.entered());
  EXPECT_FALSE(d1.exited());
  EXPECT_FALSE(d2.entered());
  EXPECT_FALSE(d2.exited());

  generator.MoveMouseToCenterOf(w2.get());
  EXPECT_TRUE(d1.entered());
  EXPECT_TRUE(d1.exited());
  EXPECT_TRUE(d2.entered());
  EXPECT_FALSE(d2.exited());
}

// Verifies that enter / exits are sent if widgets appear and are hidden
// under the current mouse position..
TEST_F(WidgetTest, MouseEnterExitWithHide) {
  MouseEnterExitWidgetDelegate d1;
  scoped_ptr<Widget> w1(
      CreateWidget(&d1, 1, gfx::Rect(10, 10, 50, 50),
                                   root_widget()));

  EventGenerator generator(root_widget());
  generator.MoveMouseToCenterOf(w1.get());
  EXPECT_TRUE(d1.entered());
  EXPECT_FALSE(d1.exited());

  MouseEnterExitWidgetDelegate d2;
  scoped_ptr<Widget> w2(CreateWidget(&d2, 2, gfx::Rect(10, 10, 50, 50),
                                     root_widget()));
  // Enters / exits can be send asynchronously.
  RunFirstFrame();
  EXPECT_TRUE(d1.entered());
  EXPECT_TRUE(d1.exited());
  EXPECT_TRUE(d2.entered());
  EXPECT_FALSE(d2.exited());

  d1.ResetExpectations();
  w2->Hide();
  // Enters / exits can be send asynchronously.
  RunFirstFrame();
  EXPECT_TRUE(d2.exited());
  EXPECT_TRUE(d1.entered());
}

TEST_F(WidgetTest, MouseEnterExitWithParentHide) {
  MouseEnterExitWidgetDelegate d1;
  scoped_ptr<Widget> w1(CreateWidget(&d1, 1, gfx::Rect(10, 10, 50, 50),
                                     root_widget()));
  MouseEnterExitWidgetDelegate d2;
  Widget* w2 = CreateWidget(&d2, 2, gfx::Rect(10, 10, 50, 50),
                                            w1.get());
  EventGenerator generator(root_widget());
  generator.MoveMouseToCenterOf(w2);
  // Enters / exits can be send asynchronously.
  RunFirstFrame();
  EXPECT_TRUE(d2.entered());
  EXPECT_FALSE(d2.exited());

  d2.ResetExpectations();
  w1->Hide();
  RunFirstFrame();
  EXPECT_FALSE(d2.entered());
  EXPECT_TRUE(d2.exited());

  w1.reset();
}

TEST_F(WidgetTest, MouseEnterExitWithParentDelete) {
  MouseEnterExitWidgetDelegate d1;
  scoped_ptr<Widget> w1(
      CreateWidget(&d1, 1, gfx::Rect(10, 10, 50, 50),
                                   root_widget()));
  MouseEnterExitWidgetDelegate d2;
  Widget* w2 = CreateWidget(&d2, 2, gfx::Rect(10, 10, 50, 50),
                                            w1.get());
  EventGenerator generator(root_widget());
  generator.MoveMouseToCenterOf(w2);

  // Enters / exits can be send asynchronously.
  RunFirstFrame();
  EXPECT_TRUE(d2.entered());
  EXPECT_FALSE(d2.exited());

  d2.ResetExpectations();
  w1.reset();
  RunFirstFrame();
  EXPECT_FALSE(d2.entered());
  EXPECT_TRUE(d2.exited());
}

// Creates a widget with a delegate (w111) that can handle events at a lower
// z-index than a widget without a delegate (w12). w12 is sized to fill the
// entire bounds of the container. This test verifies that
// GetEventHandlerForPoint() skips w12 even though its bounds contain the event,
// because it has no children that can handle the event and it has no delegate
// allowing it to handle the event itself.
TEST_F(WidgetTest, GetEventHandlerForPoint_NoDelegate) {
  TestWidgetDelegate d111;
  scoped_ptr<Widget> w1(CreateWidget(NULL, 1,
      gfx::Rect(0, 0, 500, 500), root_widget()));
  scoped_ptr<Widget> w11(CreateWidget(NULL, 11,
      gfx::Rect(0, 0, 500, 500), w1.get()));
  scoped_ptr<Widget> w111(CreateWidget(&d111, 111,
      gfx::Rect(50, 50, 450, 450), w11.get()));
  scoped_ptr<Widget> w12(CreateWidget(NULL, 12,
      gfx::Rect(0, 0, 500, 500), w1.get()));

  gfx::Point target_point = w111->bounds().CenterPoint();
  EXPECT_EQ(w111.get(), w1->GetEventHandlerForPoint(target_point));
}

TEST_F(WidgetTest, Capture) {
  gfx::Rect bounds(0, 0, 20, 20);
  CaptureWidgetDelegateImpl delegate;
  scoped_ptr<Widget> widget(CreateWidget(&delegate, bounds));
  widget->SetName("widget1");
  host_->Show();

  EXPECT_FALSE(widget->HasCapture());
  delegate.ResetCounts();

  // Do a capture.
  widget->SetCapture();
  EXPECT_TRUE(widget->HasCapture());
  EXPECT_EQ(0, delegate.capture_lost_count());
  EXPECT_EQ(0, delegate.capture_changed_event_count());
  EventGenerator generator(root_widget(), gfx::Point(50, 50));
  generator.PressLeftButton();
  EXPECT_EQ(1, delegate.mouse_event_count());
  generator.ReleaseLeftButton();

  EXPECT_EQ(2, delegate.mouse_event_count());
  delegate.ResetCounts();

  RunFirstFrame();
}

// Changes capture while capture is already ongoing.
TEST_F(WidgetTest, ChangeCaptureWhileMouseDown) {
  CaptureWidgetDelegateImpl delegate;
  scoped_ptr<Widget> widget(CreateWidget(
      &delegate, gfx::Rect(0, 0, 20, 20), root_widget()));
  CaptureWidgetDelegateImpl delegate2;
  scoped_ptr<Widget> w2(CreateWidget(
      &delegate2, gfx::Rect(20, 20, 20, 20), root_widget()));


  RunFirstFrame();
  EXPECT_FALSE(widget->HasCapture());

  // Do a capture.
  delegate.ResetCounts();
  widget->SetCapture();
  EXPECT_TRUE(widget->HasCapture());
  EXPECT_EQ(0, delegate.capture_lost_count());
  EXPECT_EQ(0, delegate.capture_changed_event_count());
  EventGenerator generator(root_widget(), gfx::Point(50, 50));
  generator.PressLeftButton();
  EXPECT_EQ(0, delegate.capture_lost_count());
  EXPECT_EQ(0, delegate.capture_changed_event_count());
  EXPECT_EQ(1, delegate.mouse_event_count());

  // Set capture to |w2|, should implicitly unset capture for |widget|.
  delegate.ResetCounts();
  delegate2.ResetCounts();
  w2->SetCapture();

  generator.MoveMouseTo(gfx::Point(40, 40), 2);
  EXPECT_EQ(1, delegate.capture_lost_count());
  EXPECT_EQ(1, delegate.capture_changed_event_count());
  EXPECT_EQ(1, delegate.mouse_event_count());
  EXPECT_EQ(2, delegate2.mouse_event_count());
}

// Verifies capture is reset when a widget is destroyed.
TEST_F(WidgetTest, ReleaseCaptureOnDestroy) {
  CaptureWidgetDelegateImpl delegate;
  scoped_ptr<Widget> widget(CreateWidget(
      &delegate, gfx::Rect(0, 0, 20, 20), root_widget()));
  EXPECT_FALSE(widget->HasCapture());

  // Do a capture.
  widget->SetCapture();
  EXPECT_TRUE(widget->HasCapture());

  // Destroy the widget.
  widget.reset();

  // Make sure the root widget doesn't reference the widget anymore.
  EXPECT_EQ(NULL, host()->dispatcher()->mouse_pressed_handler());
  EXPECT_EQ(NULL, client::GetCaptureWidget(root_widget()));
}

TEST_F(WidgetTest, GetBoundsInRootWidget) {
  scoped_ptr<Widget> viewport(CreateWidget(
      gfx::Rect(0, 0, 300, 300), root_widget()));
  scoped_ptr<Widget> child(CreateWidget(
      gfx::Rect(0, 0, 100, 100), viewport.get()));
  // Sanity check.
  EXPECT_EQ("0,0 100x100", child->GetBoundsInRootWidget().ToString());

  // The |child| widget's screen bounds should move along with the |viewport|.
  viewport->SetBounds(gfx::Rect(-100, -100, 300, 300));
  EXPECT_EQ("-100,-100 100x100", child->GetBoundsInRootWidget().ToString());

  // The |child| widget is moved to the 0,0 in screen coordinates.
  // |GetBoundsInRootWidget()| should return 0,0.
  child->SetBounds(gfx::Rect(100, 100, 100, 100));
  EXPECT_EQ("0,0 100x100", child->GetBoundsInRootWidget().ToString());
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
