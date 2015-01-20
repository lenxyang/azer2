#include "azer/ui/widget/api.h"

#include "azer/ui/widget/tests/test_base.h"
#include "azer/ui/widget/tests/test_widget_delegate.h"

namespace azer {
namespace widget {
namespace testing {

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
}

}  // namespace testing
}  // namespace widget
}  // namespace azer
