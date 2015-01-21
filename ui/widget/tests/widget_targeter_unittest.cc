#include "ui/events/test/test_event_handler.h"

#include "azer/ui/widget/api.h"
#include "azer/ui/widget/tests/test_base.h"
#include "azer/ui/widget/tests/test_widget_delegate.h"


namespace azer {
namespace widget {
namespace testing {

class WidgetTargeterTest : public WidgetTestBase {
 public:
  WidgetTargeterTest() {}
  ~WidgetTargeterTest() override {}

  void SetUp() override {
    WidgetTestBase::SetUp();
  }
  void TearDown() override {
    WidgetTestBase::TearDown();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(WidgetTargeterTest);
};

TEST_F(WidgetTargeterTest, Basic) {
  gfx::Rect bounds(0, 0, 100, 100);
  TestWidgetDelegate delegate;
  scoped_ptr<Widget> widget(CreateWidget(&delegate, bounds));
  widget->SetName("widget1");
  host_->Show();

  ui::test::TestEventHandler handler;
  widget->AddPreTargetHandler(&handler);

  ui::MouseEvent press(ui::ET_MOUSE_PRESSED,
                       gfx::Point(20, 20),
                       gfx::Point(20, 20),
                       ui::EF_NONE,
                       ui::EF_NONE);
  DispatchEventUsingWidgetDispatcher(&press);
  EXPECT_EQ(1, handler.num_mouse_events());
}

TEST_F(WidgetTargeterTest, SubWindow) {
  gfx::Rect bounds1(400, 300, 100, 100);
  WidgetEventCounter delegate1;
  scoped_ptr<Widget> widget1(CreateWidget(&delegate1, bounds1));
  widget1->SetName("widget1");

  gfx::Rect bounds2(80, 80, 10, 10);
  WidgetEventCounter delegate2;
  scoped_ptr<Widget> widget2(CreateWidget(&delegate2, bounds2, widget1.get()));
  widget2->SetName("widget2");

  ui::test::TestEventHandler handler;
  host_->widget()->AddPreTargetHandler(&handler);
  host_->Show();

  ui::MouseEvent press1(ui::ET_MOUSE_PRESSED,
                        gfx::Point(410, 310),
                        gfx::Point(410, 310),
                        ui::EF_NONE,
                        ui::EF_NONE);
  DispatchEventUsingWidgetDispatcher(&press1);
  EXPECT_EQ(1, handler.num_mouse_events());
  EXPECT_EQ(1, delegate1.num_mouse_events());
  EXPECT_EQ(0, delegate2.num_mouse_events());


  handler.Reset();
  delegate1.Reset();
  delegate2.Reset();
  ui::MouseEvent press2(ui::ET_MOUSE_PRESSED,
                        gfx::Point(480, 380),
                        gfx::Point(480, 380),
                        ui::EF_NONE,
                        ui::EF_NONE);
  DispatchEventUsingWidgetDispatcher(&press2);
  EXPECT_EQ(1, handler.num_mouse_events());
  EXPECT_EQ(0, delegate1.num_mouse_events());
  EXPECT_EQ(1, delegate2.num_mouse_events());

  handler.Reset();
  delegate1.Reset();
  delegate2.Reset();
  ui::MouseEvent press3(ui::ET_MOUSE_PRESSED,
                        gfx::Point(10, 10),
                        gfx::Point(10, 10),
                        ui::EF_NONE,
                        ui::EF_NONE);
  DispatchEventUsingWidgetDispatcher(&press3);
  EXPECT_EQ(1, handler.num_mouse_events());
  EXPECT_EQ(0, delegate1.num_mouse_events());
  EXPECT_EQ(0, delegate2.num_mouse_events());
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
