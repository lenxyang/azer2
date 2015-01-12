#include "azer/ui/win/window.h"
#include "azer/ui/win/tests/test_base.h"

class WindowTest : public azer::ui::WinTestBase {
 public:
  WindowTest() : max_separation_(0) {
  }

  void SetUp() override {
    AuraTestBase::SetUp();
    // TODO: there needs to be an easier way to do this.
    max_separation_ = ui::GestureConfiguration::GetInstance()
        ->max_separation_for_gesture_touches_in_pixels();
    ui::GestureConfiguration::GetInstance()
        ->set_max_separation_for_gesture_touches_in_pixels(0);
  }

  void TearDown() override {
    AuraTestBase::TearDown();
    ui::GestureConfiguration::GetInstance()
        ->set_max_separation_for_gesture_touches_in_pixels(max_separation_);
  }

 private:
  float max_separation_;

  DISALLOW_COPY_AND_ASSIGN(WindowTest);
};
