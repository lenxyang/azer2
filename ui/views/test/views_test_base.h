#pragma once

#include "base/basictypes.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/widget/widget.h"

namespace views {

class AuraScreen;

class ViewsTestBase : public testing::Test {
 public:
  ViewsTestBase();
  ~ViewsTestBase() override;
  void SetUp() override;
  void TearDown() override;

  
 protected:
  bool setup_called_;
  bool teardown_called_;
  scoped_ptr<AuraScreen> aura_screen_;
  DISALLOW_COPY_AND_ASSIGN(ViewsTestBase);
};
}  // namespace views
