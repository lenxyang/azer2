
#include "base/memory/scoped_ptr.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/win/window.h"
#include "azer/ui/win/tests/test_base.h"
#include "azer/ui/win/tests/test_window.h"
#include "azer/ui/win/tests/window_delegate_for_test.h"

namespace azer {
namespace win {

class WindowTest : public WinTestBase {
 public:
  WindowTest() : max_separation_(0) {
  }

  void SetUp() override {
    WinTestBase::SetUp();
  }

  void TearDown() override {
    WinTestBase::TearDown();
  }

 private:
  float max_separation_;

  DISALLOW_COPY_AND_ASSIGN(WindowTest);
};

TEST_F(WindowTest, Base) {
  host_->Show();
}

TEST_F(WindowTest, Contains) {
  Window parent(NULL);
  Window child1(NULL);
  Window child2(NULL);

  parent.AddChild(&child1);
  child1.AddChild(&child2);

  EXPECT_TRUE(parent.Contains(&parent));
  EXPECT_TRUE(parent.Contains(&child1));
  EXPECT_TRUE(parent.Contains(&child2));

  EXPECT_FALSE(parent.Contains(NULL));
  EXPECT_FALSE(child1.Contains(&parent));
  EXPECT_FALSE(child2.Contains(&child1));
}

TEST_F(WindowTest, MousePressedAndRelease) {
}

}  // namespace win
}  // namespace azer
