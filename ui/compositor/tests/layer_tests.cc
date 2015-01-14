#include "azer/ui/compositor/tests/test_base.h"

namespace azer {
namespace compositor {

class LayerTest : public CompositorTestBase {
 public:
  LayerTest() : max_separation_(0) {
  }

  void SetUp() override {
    WinTestBase::SetUp();
  }

  void TearDown() override {
    WinTestBase::TearDown();
  }

 private:
  float max_separation_;

  DISALLOW_COPY_AND_ASSIGN(LayerTest);
};

TEST_F(LayerTest, Base) {
}

}  // namespace compositor
}  // namespace azer
