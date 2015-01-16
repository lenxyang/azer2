#include "azer/ui/widget/api.h"

#include "azer/ui/widget/tests/test_base.h"

namespace azer {
namespace widget {

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

}  // namespace widget
}  // namespace azer
