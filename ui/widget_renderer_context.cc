#include "azer/ui/widget_renderer_context.h"

#include "base/logging.h"
#include "ui/aura/window_tree_host.h"
#include "ui/aura/window.h"
#include "ui/compositor/compositor.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"
#include "azer/ui/widget_util.h"
#include "azer/ui/adapter/output_device.h"

namespace azer {
WidgetRendererContext::WidgetRendererContext(views::Widget* widget) 
    : render_system_(NULL)
    , widget_(widget) {
  gfx::NativeView native_view = widget->GetNativeView();
  aura::WindowTreeHost* aura_host = ((aura::Window*)native_view)->GetHost();
  gfx::AcceleratedWidget acc_widget = aura_host->GetAcceleratedWidget();

  surface_ = new azer::Surface(acc_widget);
  render_system_ = azer::RenderSystem::Current();
  CHECK(render_system_);
  swapchain_ = render_system_->CreateSwapChainForSurface(surface_.get());
  renderer_ = swapchain_->GetRenderer();
  overlay_ = render_system_->CreateOverlay();
  ui::Compositor* compositor = widget->GetCompositor();
  renderer_->SetViewport(azer::Renderer::Viewport(gfx::Rect(compositor->size())));
}

WidgetRendererContext::~WidgetRendererContext() {
  overlay_ = NULL;
  renderer_ = NULL;
  swapchain_ = NULL;
}

void WidgetRendererContext::RenderUI() {
  renderer_->Use();
  DCHECK(widget_);
  TextureOutputDevice* device = GetTextureOutputDeviceForWidget(widget_);
  DCHECK(device);
  azer::Canvas2DPtr canvas = device->GetCanvas();
  azer::TexturePtr& texture = canvas->GetTexture();
  overlay_->SetTexture(texture);
  overlay_->SetBounds(gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f));
  overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));
  overlay_->Render(renderer_.get());
}


void WidgetRendererContext::Present() {
  swapchain_->Present();
}

}  // namespace azer
