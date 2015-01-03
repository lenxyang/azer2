#include "azer/uisbox/view/base/azer.h"

#include "azer/render/util/render_system_loader.h"
#include "ui/views/widget/widget.h"

#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"

namespace azer {

bool InitializeAzer(views::Widget* widget) {
  gfx::NativeView native_view = widget->GetNativeView();
  aura::WindowTreeHost* aura_host = ((aura::Window*)native_view)->GetHost();
  gfx::AcceleratedWidget acc_widget = aura_host->GetAcceleratedWidget();
  return azer::LoadRenderSystem(acc_widget);
}

}  // namespace azer
