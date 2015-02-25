#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "azer/ui/views/env.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/root_view.h"

namespace views {
namespace test {
class ViewsTestBase : public testing::Test {
 public:
  ViewsTestBase();
  ~ViewsTestBase() override;

  void SetUp() override;
  void TearDown() override;

  void InitRootView(const RootView::InitParams& params);
 private:
  bool setup_called_;
  bool teardown_called_;
  scoped_ptr<RootView> view_view_;
  DISALLOW_COPY_AND_ASSIGN(ViewsTestBase);
};
}  // namespace test
}  // namespace views
