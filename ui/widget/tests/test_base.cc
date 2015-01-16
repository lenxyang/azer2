#include "azer/ui/widget/tests/test_base.h"

#include "base/run_loop.h"
#include "azer/ui/widget/api.h"

namespace azer {
namespace widget {

WidgetTestBase::WidgetTestBase() {
}

WidgetTestBase::~WidgetTestBase() {
}

void WidgetTestBase::SetUp() {
  WidgetContext::Init();
  host_.reset(WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  host_->Show();

  render_loop_.reset(new RenderLoop(this));
}

void WidgetTestBase::TearDown() {
  WidgetContext::Destroy();
}

Widget* WidgetTestBase::root() {
  return host_->root();
}

}  // namespace win
}  // namespace azer
