#include "azer/render/compositor/tests/test_base.h"

namespace azer {
namespace compositor {
CompositorTestBase::CompositorTestBase() {
}

CompositorTestBase::~CompositorTestBase() {
}

void CompositorTestBase::SetUp() {
  compositor_.reset(new Compositor);
}

void CompositorTestBase::TearDown() {
}

}  // namespace compositor
}  // namespace azer
