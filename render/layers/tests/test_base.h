#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/render/compositor/layer.h"
#include "azer/render/compositor/layer_tree_host.h"
#include "azer/render/compositor/compositor.h"

namespace azer {
namespace compositor {

class Compositor;

class CompositorTestBase : public testing::Test {
 public:
  CompositorTestBase();
  ~CompositorTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  scoped_ptr<Compositor> compositor_;
  base::MessageLoopForUI message_loop_;
  DISALLOW_COPY_AND_ASSIGN(CompositorTestBase);
};
}  // namespace compositor
}  // namespace azer
