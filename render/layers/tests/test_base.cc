#include "azer/render/layers/tests/test_base.h"

namespace azer {
namespace layers {
LayersTestBase::LayersTestBase() {
}

LayersTestBase::~LayersTestBase() {
}

void LayersTestBase::SetUp() {
  tree_host_.reset(new LayerTreeHost);
}

void LayersTestBase::TearDown() {
}

}  // namespace layers
}  // namespace azer
