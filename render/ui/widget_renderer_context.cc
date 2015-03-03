#include "azer/uisbox/adapter/base/widget_renderer_context.h"

#include "base/logging.h"
#include "ui/views/widget/widget.h"
#include "ui/aura/window_tree_host.h"
#include "ui/aura/window.h"
#include "ui/compositor/compositor.h"
#include "azer/render/render_system.h"
#include "azer/render/canvas2d.h"
#include "azer/uisbox/adapter/base/widget_util.h"
#include "azer/uisbox/adapter/base/output_device.h"


WidgetRendererContext::WidgetRendererContext(views::Widget* widget) 
    : render_system_(NULL)
    , widget_(widget) {
  gfx::NativeView native_view = widget->GetNativeView();
  aura::WindowTreeHost* aura_host = ((aura::Window*)native_view)->GetHost();
  gfx::AcceleratedWidget acc_widget = aura_host->GetAcceleratedWidget();

  surface_.reset(new azer::Surface(acc_widget));
  render_system_ = azer::RenderSystem::Current();
  CHECK(render_system_);
  swapchain_.reset(render_system_->CreateSwapChainForSurface(surface_.get()));
  renderer_ = swapchain_->GetRenderer();
  overlay_.reset(render_system_->CreateOverlay());
  ui::Compositor* compositor = widget->GetCompositor();
  renderer_->SetViewport(azer::Renderer::Viewport(gfx::Rect(compositor->size())));
}

WidgetRendererContext::~WidgetRendererContext() {
  overlay_.reset();
  renderer_.reset();
  swapchain_.reset();
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
