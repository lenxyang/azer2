#include "azer/render/layers/tests/test_base.h"

namespace azer {
namespace layers {

class LayerTest : public LayersTestBase {
 public:
  LayerTest() : max_separation_(0) {
  }

  void SetUp() override {
    LayersTestBase::SetUp();
  }

  void TearDown() override {
    LayersTestBase::TearDown();
  }

 private:
  float max_separation_;

  DISALLOW_COPY_AND_ASSIGN(LayerTest);
};

TEST_F(LayerTest, Base) {
}

}  // namespace layers
}  // namespace azer
