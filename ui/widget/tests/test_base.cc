#include "azer/ui/widget/tests/test_base.h"

#include "base/run_loop.h"
#include "azer/ui/widget/context.h"
#include "azer/ui/widget/widget_tree_host.h"

namespace azer {
namespace widget {

WidgetTestBase::WidgetTestBase() {
}

WidgetTestBase::~WidgetTestBase() {
}

void WidgetTestBase::SetUp() {
  WidgetContext::CreateInstance(true);
  host_.reset(WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  host_->Show();

  renderloop_.reset(new REnderLoop(this));
}

void WidgetTestBase::TearDown() {
  WidgetContext::DeleteInstance();
}

void WidgetTestBase::RunAllPendingInMessageLoop() {
  base::RunLoop run_loop;
  run_loop.RunUntilIdle();
}

Widget* WidgetTestBase::root() {
  return host_->root();
}

}  // namespace win
}  // namespace azer
