#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/widget/context.h"
#include "azer/ui/widget/render_loop.h"
#include "azer/ui/widget/widget_tree_host.h"


namespace azer {
namespace widget {

class WidgetTreeHost;

class WidgetTestBase : public testing::Test
                     , public RenderLoopDelegate {
 public:
  WidgetTestBase();
  ~WidgetTestBase() override;

  void SetUp() override;
  void TearDown() override;
 protected:
  Widget* root();
  WidgetTreeHost* host() { return host_.get();}
  void RunAllPendingInMessageLoop();

  base::MessageLoopForUI message_loop_;
  scoped_ptr<WidgetTreeHost> host_;
  scoped_ptr<RenderLoop> render_loop_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTestBase);
};
}  // namespace widget
}  // namespace azer
