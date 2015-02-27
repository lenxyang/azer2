#include "base/memory/scoped_ptr.h"
#include "base/rand_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/events/event.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/transform.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/strings/grit/ui_strings.h"

#include "azer/ui/views/view.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/ui/views/tests/views_test_base.h"

namespace views {

typedef test::ViewsTestBase ViewTest;

// A derived class for testing purpose.
class TestView : public View {
 public:
  TestView()
      : View(),
        delete_on_pressed_(false),
        native_theme_(NULL),
        can_process_events_within_subtree_(true) {}
  ~TestView() override {}

  // Reset all test state
  void Reset() {
    did_change_bounds_ = false;
    last_mouse_event_type_ = 0;
    location_.SetPoint(0, 0);
    received_mouse_enter_ = false;
    received_mouse_exit_ = false;
    last_clip_.setEmpty();
    accelerator_count_map_.clear();
    can_process_events_within_subtree_ = true;
  }

  // Exposed as public for testing.
  void DoFocus() {
    views::View::Focus();
  }

  void DoBlur() {
    views::View::Blur();
  }

  bool focusable() const { return View::focusable(); }

  void set_can_process_events_within_subtree(bool can_process) {
    can_process_events_within_subtree_ = can_process;
  }

  bool CanProcessEventsWithinSubtree() const override {
    return can_process_events_within_subtree_;
  }

  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

  // void OnPaint(gfx::Canvas* canvas) override;
  // void SchedulePaintInRect(const gfx::Rect& rect) override;
  // bool AcceleratorPressed(const ui::Accelerator& accelerator) override;
  
  // void OnNativeThemeChanged(const ui::NativeTheme* native_theme) override;

  // OnBoundsChanged.
  bool did_change_bounds_;
  gfx::Rect new_bounds_;

  // MouseEvent.
  int last_mouse_event_type_;
  gfx::Point location_;
  bool received_mouse_enter_;
  bool received_mouse_exit_;
  bool delete_on_pressed_;

  // Painting.
  std::vector<gfx::Rect> scheduled_paint_rects_;

  // Painting.
  SkRect last_clip_;

  // Accelerators.
  std::map<ui::Accelerator, int> accelerator_count_map_;

  // Native theme.
  const ui::NativeTheme* native_theme_;

  // Value to return from CanProcessEventsWithinSubtree().
  bool can_process_events_within_subtree_;
};

////////////////////////////////////////////////////////////////////////////////
// OnBoundsChanged
////////////////////////////////////////////////////////////////////////////////

void TestView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  did_change_bounds_ = true;
  new_bounds_ = bounds();
}

TEST_F(ViewTest, OnBoundsChanged) {
  TestView v;

  gfx::Rect prev_rect(0, 0, 200, 200);
  gfx::Rect new_rect(100, 100, 250, 250);

  v.SetBoundsRect(prev_rect);
  v.Reset();
  v.SetBoundsRect(new_rect);

  EXPECT_TRUE(v.did_change_bounds_);
  EXPECT_EQ(v.new_bounds_, new_rect);
  EXPECT_EQ(v.bounds(), new_rect);
}

////////////////////////////////////////////////////////////////////////////////
// MouseEvent
////////////////////////////////////////////////////////////////////////////////

bool TestView::OnMousePressed(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  if (delete_on_pressed_)
    delete this;
  return true;
}

bool TestView::OnMouseDragged(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  return true;
}

void TestView::OnMouseReleased(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
}

void TestView::OnMouseEntered(const ui::MouseEvent& event) {
  received_mouse_enter_ = true;
}

void TestView::OnMouseExited(const ui::MouseEvent& event) {
  received_mouse_exit_ = true;
}

TEST_F(ViewTest, MouseEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);

  v1->Reset();
  v2->Reset();

  gfx::Point p1(110, 120);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1,
                         ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  SendEvent(&pressed, widget.get());
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_PRESSED);
  EXPECT_EQ(v2->location_.x(), 10);
  EXPECT_EQ(v2->location_.y(), 20);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Drag event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  gfx::Point p2(50, 40);
  ui::MouseEvent dragged(ui::ET_MOUSE_DRAGGED, p2, p2,
                         ui::EF_LEFT_MOUSE_BUTTON, 0);
  SendEvent(&dragged, widget.get());
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_DRAGGED);
  EXPECT_EQ(v2->location_.x(), -50);
  EXPECT_EQ(v2->location_.y(), -60);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Releasted event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(), 0,
                          0);
  SendEvent(&released, widget.get());
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_RELEASED);
  EXPECT_EQ(v2->location_.x(), -100);
  EXPECT_EQ(v2->location_.y(), -100);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  widget->CloseNow();
}

TEST_F(ViewTest, NotifyEnterExitOnChild) {
  scoped_ptr<Widget> widget(new Widget);
  Widget::InitParams params;
  params.bounds = gfx::Rect(1, 1);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  // Have this hierarchy of views (the coords here are in root coord):
  // v1 (0, 0, 100, 100)
  //  - v11 (0, 0, 20, 30)
  //    - v111 (5, 5, 5, 15)
  //  - v12 (50, 10, 30, 90)
  //    - v121 (60, 20, 10, 10)
  // v2 (105, 0, 100, 100)
  //  - v21 (120, 10, 50, 20)

  TestView* v1 = new TestView;
  v1->SetBounds(0, 0, 100, 100);
  root_view->AddChildView(v1);
  v1->set_notify_enter_exit_on_child(true);

  TestView* v11 = new TestView;
  v11->SetBounds(0, 0, 20, 30);
  v1->AddChildView(v11);

  TestView* v111 = new TestView;
  v111->SetBounds(5, 5, 5, 15);
  v11->AddChildView(v111);

  TestView* v12 = new TestView;
  v12->SetBounds(50, 10, 30, 90);
  v1->AddChildView(v12);

  TestView* v121 = new TestView;
  v121->SetBounds(10, 10, 10, 10);
  v12->AddChildView(v121);

  TestView* v2 = new TestView;
  v2->SetBounds(105, 0, 100, 100);
  root_view->AddChildView(v2);

  TestView* v21 = new TestView;
  v21->SetBounds(15, 10, 50, 20);
  v2->AddChildView(v21);

  v1->Reset();
  v11->Reset();
  v111->Reset();
  v12->Reset();
  v121->Reset();
  v2->Reset();
  v21->Reset();

  // Move the mouse in v111.
  gfx::Point p1(6, 6);
  ui::MouseEvent move1(ui::ET_MOUSE_MOVED, p1, p1, 0, 0);
  SendEvent(&move1, widget.get());
  EXPECT_TRUE(v111->received_mouse_enter_);
  EXPECT_FALSE(v11->last_mouse_event_type_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v111->Reset();
  v1->Reset();

  // Now, move into v121.
  gfx::Point p2(65, 21);
  ui::MouseEvent move2(ui::ET_MOUSE_MOVED, p2, p2, 0, 0);
  SendEvent(&move2, widget.get());
  EXPECT_TRUE(v111->received_mouse_exit_);
  EXPECT_TRUE(v121->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v111->Reset();
  v121->Reset();

  // Now, move into v11.
  gfx::Point p3(1, 1);
  ui::MouseEvent move3(ui::ET_MOUSE_MOVED, p3, p3, 0, 0);
  SendEvent(&move3, widget.get());
  EXPECT_TRUE(v121->received_mouse_exit_);
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v121->Reset();
  v11->Reset();

  // Move to v21.
  gfx::Point p4(121, 15);
  ui::MouseEvent move4(ui::ET_MOUSE_MOVED, p4, p4, 0, 0);
  SendEvent(&move4, widget.get());
  EXPECT_TRUE(v21->received_mouse_enter_);
  EXPECT_FALSE(v2->last_mouse_event_type_);
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_exit_);

  v21->Reset();
  v11->Reset();
  v1->Reset();

  // Move to v1.
  gfx::Point p5(21, 0);
  ui::MouseEvent move5(ui::ET_MOUSE_MOVED, p5, p5, 0, 0);
  SendEvent(&move5, widget.get());
  EXPECT_TRUE(v21->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v21->Reset();
  v1->Reset();

  // Now, move into v11.
  gfx::Point p6(15, 15);
  ui::MouseEvent mouse6(ui::ET_MOUSE_MOVED, p6, p6, 0, 0);
  SendEvent(&mouse6, widget.get());
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v11->Reset();
  v1->Reset();

  // Move back into v1. Although |v1| had already received an ENTER for mouse6,
  // and the mouse remains inside |v1| the whole time, it receives another ENTER
  // when the mouse leaves v11.
  gfx::Point p7(21, 0);
  ui::MouseEvent mouse7(ui::ET_MOUSE_MOVED, p7, p7, 0, 0);
  SendEvent(&mouse7, widget.get());
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_FALSE(v1->received_mouse_enter_);

  widget->CloseNow();
}



}  // namespace views
