#include "azer/ui/widget/tests/test_base.h"

#include "base/run_loop.h"
#include "azer/ui/widget/context.h"
#include "azer/ui/widget/widget_tree_host.h"
#include "azer/render/util/render_system_loader.h"

namespace azer {
namespace widget {

WidgetTestBase::WidgetTestBase() {
}

WidgetTestBase::~WidgetTestBase() {
}

void WidgetTestBase::SetUp() {
  WidgetContext::CreateInstance(true);
  host_.reset(WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  CHECK(azer::LoadRenderSystem(
      (gfx::AcceleratedWidget)host_->GetAcceleratedWidget()));
}

void WidgetTestBase::TearDown() {
  WidgetContext::DeleteInstance();
}

void WidgetTestBase::RunAllPendingInMessageLoop() {
  base::RunLoop run_loop;
  run_loop.RunUntilIdle();
}

Widget* WidgetTestBase::root_widget() {
  return host_->root();
}

}  // namespace win
}  // namespace azer
