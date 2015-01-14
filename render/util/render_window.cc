#include "azer/render/util/render_window.h"
#include "azer/ui/win/window_tree_host.h"

#include "base/logging.h"

namespace azer {
namespace util {

RenderWindow::RenderWindow(const gfx::Rect& bounds)
    : bounds_(bounds) {
}

RenderWindow::~RenderWindow() {
}

void RenderWindow::Loop(RenderLoop::Delegate* delegate) {
  scoped_ptr<RenderLoop> renderloop(new RenderLoop(delegate));
  scoped_ptr<win::WinContext> win_context(win::WinContext::CreateInstance(true));
  scoped_ptr<win::WindowTreeHost> host(win::WindowTreeHost::Create(bounds_));

  CHECK(azer::LoadRenderSystem(
      (gfx::AcceleratedWidget)host->GetAcceleratedWidget()));
  renderloop->Run();

  host.reset();
  win_context.reset();
}

}  // namespace util
}  // namespace azer
