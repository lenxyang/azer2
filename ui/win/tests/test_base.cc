#include "azer/ui/win/tests/test_base.h"

namespace azer {
namespace win {

WinTestBase::WinTestBase() {
}

WinTestBase::~WinTestBase() {
}

void WinTestBase::SetUp() {
  host_.reset(WindowTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
}

void WinTestBase::TearDown() {
}
}  // namespace win
}  // namespace azer
