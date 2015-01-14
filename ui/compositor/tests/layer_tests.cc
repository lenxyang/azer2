#include "azer/ui/compositor/tests/test_base.h"

namespace azer {
namespace base {

class LayerTest : public WinTestBase {
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

}  // namespace base
}  // namespace azer
