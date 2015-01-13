#include "azer/ui/win/tests/test_base.h"

#include "azer/ui/win/context.h"

namespace azer {
namespace win {

WinTestBase::WinTestBase() {
}

WinTestBase::~WinTestBase() {
}

void WinTestBase::SetUp() {
  WinContext::CreateInstance(true);
  host_.reset(WindowTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
}

void WinTestBase::TearDown() {
  WinContext::DeleteInstance();
}
}  // namespace win
}  // namespace azer
