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
  scoped_ptr<Widget> widget(CreateWidget(bounds, &delegate));
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
  scoped_ptr<Widget> widget1(CreateWidget(bounds1, &delegate1));
  widget1->SetName("widget1");

  gfx::Rect bounds2(80, 80, 10, 10);
  WidgetEventCounter delegate2;
  scoped_ptr<Widget> widget2(CreateWidget(bounds2, widget1.get(), &delegate2));
  widget2->SetName("widget2");
  host_->Show();

  ui::MouseEvent press(ui::ET_MOUSE_PRESSED,
                       gfx::Point(480, 380),
                       gfx::Point(480, 380),
                       ui::EF_NONE,
                       ui::EF_NONE);
  DispatchEventUsingWidgetDispatcher(&press);
  EXPECT_EQ(0, delegate1.num_mouse_events());
  EXPECT_EQ(1, delegate2.num_mouse_events());
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
