#include "azer/ui/widget/api.h"

#include "azer/ui/widget/tests/test_base.h"

namespace azer {
namespace widget {

class WidgetTargeterTest : public WidgetTargeterTestBase {
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
}

}  // namespace widget
}  // namespace azer
