#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/context.h"

namespace azer {
namespace widget {

class WidgetTreeHost;

class WidgetTestBase : public testing::Test {
 public:
  WidgetTestBase();
  ~WidgetTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  Widget* root_widget();

  base::MessageLoopForUI message_loop_;
  scoped_ptr<WidgetTreeHost> host_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTestBase);
};
}  // namespace widget
}  // namespace azer
