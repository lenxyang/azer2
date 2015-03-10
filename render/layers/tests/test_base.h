#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/render/layers/layer.h"
#include "azer/render/layers/layer_tree_host.h"

namespace azer {
namespace layers {

class Compositor;

class LayersTestBase : public testing::Test {
 public:
  LayersTestBase();
  ~LayersTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  scoped_ptr<LayerTreeHost> tree_host_;
  base::MessageLoopForUI message_loop_;
  DISALLOW_COPY_AND_ASSIGN(LayersTestBase);
};
}  // namespace layers
}  // namespace azer
