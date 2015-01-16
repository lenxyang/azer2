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
  gfx::Rect bounds(100, 100, 300, 300);
  ColorWidgetDelegate delegate(SK_ColorRED);
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
  DispatchEventUsingWindowDispatcher(&press);
  EXPECT_EQ(1, handler.num_mouse_events());
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
