
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

TEST_F(WindowTest, ContainsPointInRoot) {
  scoped_ptr<Window> w(
      CreateTestWindow(SK_ColorWHITE, 1, gfx::Rect(10, 10, 5, 5),
                       root_window()));
  EXPECT_FALSE(w->ContainsPointInRoot(gfx::Point(9, 9)));
  EXPECT_TRUE(w->ContainsPointInRoot(gfx::Point(10, 10)));
  EXPECT_TRUE(w->ContainsPointInRoot(gfx::Point(14, 14)));
  EXPECT_FALSE(w->ContainsPointInRoot(gfx::Point(15, 15)));
  EXPECT_FALSE(w->ContainsPointInRoot(gfx::Point(20, 20)));
}

TEST_F(WindowTest, ContainsPoint) {
  scoped_ptr<Window> w(
      CreateTestWindow(SK_ColorWHITE, 1, gfx::Rect(10, 10, 5, 5),
                       root_window()));
  EXPECT_TRUE(w->ContainsPoint(gfx::Point(0, 0)));
  EXPECT_TRUE(w->ContainsPoint(gfx::Point(4, 4)));
  EXPECT_FALSE(w->ContainsPoint(gfx::Point(5, 5)));
  EXPECT_FALSE(w->ContainsPoint(gfx::Point(10, 10)));
}

TEST_F(WindowTest, ConvertPointToWindow) {
  // Window::ConvertPointToWindow is mostly identical to
  // Layer::ConvertPointToLayer, except NULL values for |source| are permitted,
  // in which case the function just returns.
  scoped_ptr<Window> w1(CreateTestWindowWithId(1, root_window()));
  gfx::Point reference_point(100, 100);
  gfx::Point test_point = reference_point;
  Window::ConvertPointToTarget(NULL, w1.get(), &test_point);
  EXPECT_EQ(reference_point, test_point);
}

TEST_F(WindowTest, MousePressedAndRelease) {
}

}  // namespace win
}  // namespace azer
