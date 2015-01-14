
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

TEST_F(WindowTest, MousePressedAndRelease) {
  Window
}

}  // namespace win
}  // namespace azer
