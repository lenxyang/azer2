#pragma once

#include "base/basictypes.h"
#include "testing/gtest/gtest.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/widget/widget.h"

namespace veiws {
class ViewsTestBase : public testing::Test {
 public:
  ~ViewsTestBase() override;
  void SetUp() override;
  void TearDown() override;

  
 protected:
  bool setup_called_;
  bool teardown_called_;
  DISALLOW_COPY_AND_ASSIGN(ViewsTestBase);
};
}  // namespace views
