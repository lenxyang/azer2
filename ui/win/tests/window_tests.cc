
#include "base/memory/scoped_ptr.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/win/window.h"
#include "azer/ui/win/tests/test_base.h"

using azer::win::WindowTreeHost;

class WindowTest : public azer::win::WinTestBase {
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
