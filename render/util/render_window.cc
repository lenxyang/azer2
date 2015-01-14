#include "azer/render/util/render_window.h"

#include "azer/base/render_loop.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/win/context.h"

#include "base/logging.h"

namespace azer {
namespace util {

RenderWindow::RenderWindow(const gfx::Rect& bounds)
    : bounds_(bounds) {
}

RenderWindow::~RenderWindow() {
}

void RenderWindow::Loop(RenderLoop::Delegate* delegate) {
  scoped_refptr<RenderLoop> renderloop(new RenderLoop(delegate));
  win::WinContext::CreateInstance(true);
  scoped_ptr<win::WindowTreeHost> host(win::WindowTreeHost::Create(bounds_));

  CHECK(azer::LoadRenderSystem(
      (gfx::AcceleratedWidget)host->GetAcceleratedWidget()));
  renderloop->Run();

  host.reset();
  win::WinContext::DeleteInstance();
}

}  // namespace util
}  // namespace azer
