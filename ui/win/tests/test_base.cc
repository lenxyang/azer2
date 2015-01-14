#include "azer/ui/win/tests/test_base.h"

#include "base/run_loop.h"
#include "azer/ui/win/context.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/render/util/render_system_loader.h"

namespace azer {
namespace win {

WinTestBase::WinTestBase() {
}

WinTestBase::~WinTestBase() {
}

void WinTestBase::SetUp() {
  WinContext::CreateInstance(true);
  host_.reset(WindowTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  CHECK(azer::LoadRenderSystem(
      (gfx::AcceleratedWidget)host_->GetAcceleratedWidget()));
}

void WinTestBase::TearDown() {
  WinContext::DeleteInstance();
}

void WinTestBase::RunAllPendingInMessageLoop() {
  base::RunLoop run_loop;
  run_loop.RunUntilIdle();
}

Window* WinTestBase::root_window() {
  return host_->window();
}

bool WinTestBase::DispatchEventUsingWindowDispatcher(::ui::Event* event) {
  ui::EventDispatchDetails details =
      event_processor()->OnEventFromSource(event);
  CHECK(!details.dispatcher_destroyed);
  return event->handled();
}
}  // namespace win
}  // namespace azer
