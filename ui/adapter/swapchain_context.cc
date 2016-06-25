#include "azer/ui/adapter/swapchain_context.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/trace_event/trace_event.h"
#include "ui/aura/window_tree_host.h"
#include "ui/aura/window.h"
#include "ui/compositor/compositor.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/widget/widget.h"

#include "azer/azer.h"
#include "azer/render/canvas2d.h"
#include "azer/ui/adapter/output_device.h"
#include "azer/ui/adapter/util.h"

namespace azer {

SwapchainContext::SwapchainContext(views::Widget* widget) 
    : render_system_(NULL),
      widget_(widget) {
  render_system_ = RenderSystem::Current();
  CHECK(render_system_);
  widget_->AddObserver(this);
  SetSwapchainContext(widget_, this);
  rasterizer_state_ = render_system_->CreateRasterizerState();
  depth_state_ = render_system_->CreateDepthStencilState();
  rasterizer_state_->SetFrontFace(kCounterClockwise);
  rasterizer_state_->SetCullingMode(kCullNone);
  depth_state_->EnableDepthTest(false);

  TRACE_EVENT_OBJECT_CREATED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::SwapchainContext", id_);
}

SwapchainContext::~SwapchainContext() {
  TRACE_EVENT0("azer", "SwapchainContext::~SwapchainContext()");
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::SwapchainContext", id_);

  widget_->RemoveObserver(this);
  Clear();
}

void SwapchainContext::Clear() {
  overlay_ = NULL;
  renderer_ = NULL;
  swapchain_ = NULL;
}

void SwapchainContext::Reset() {
  TRACE_EVENT0("azer", "SwapchainContext::Reset()");
  surface_ = CreateSurfaceForWidget(widget_);
  swapchain_ = render_system_->CreateSwapChainForSurface(surface_.get());
  ResetSwapchain();
}

void SwapchainContext::ResetSwapchain() {
  TRACE_EVENT0("azer", "SwapchainContext::ResetSwapchain()");
  renderer_ = swapchain_->GetRenderer();
  overlay_ = new Overlay;
  renderer_->SetViewport(Viewport(gfx::Rect(surface_->GetBounds().size())));
}

Surface* SwapchainContext::CreateSurfaceForWidget(views::Widget* widget) {
  gfx::NativeView native_view = widget->GetNativeView();
  aura::WindowTreeHost* aura_host = ((aura::Window*)native_view)->GetHost();
  gfx::AcceleratedWidget acc_widget = aura_host->GetAcceleratedWidget();

  Surface* surface = new Surface(acc_widget);
  surface->SetExpectedSampleDesc(sample_desc_);
  return surface;
}

void SwapchainContext::RenderUI() {
  renderer_->Use();
  DCHECK(widget_);
  TextureOutputDevice* device = GetTextureOutputDeviceForWidget(widget_);
  DCHECK(device);

  ScopedRasterizerState scoped_rasterizer_state(renderer_.get());
  ScopedDepthStencilState scoped_depth_state(renderer_.get());
  renderer_->SetRasterizerState(rasterizer_state_.get());
  renderer_->SetDepthStencilState(depth_state_.get(), 0);
  Canvas2DPtr canvas = device->GetCanvas();
  if (canvas.get()) {
    TextureView* texview = canvas->GetTextureView();
    overlay_->SetTexture(texview);
    overlay_->SetBounds(gfx::RectF(0.0f, 0.0f, 1.0f, 1.0f));
    overlay_->SetTexBounds(gfx::RectF(0.0f, 0.0f, 1.0f, 1.0f));
    overlay_->Render(renderer_.get());
  }
}

void SwapchainContext::Present() {
  TRACE_EVENT0("azer", "SwapchainContext::Present()");
  swapchain_->Present();
}

// 
void SwapchainContext::OnWidgetBoundsChanged(views::Widget* widget, 
                                             const gfx::Rect& new_bounds) {
  views::NonClientView* nonclient = widget->non_client_view();
  views::NonClientFrameView *frame_view = nonclient->frame_view();
  gfx::Rect client = frame_view->GetBoundsForClientView();
  if (surface_->GetBounds().size() != client.size()
      && !client.size().IsEmpty()) {
    TRACE_EVENT0("azer", ::base::StringPrintf(
        "SwapchainContext::OnWidgetBoundsChanged(%d, %d)", client.width(), client.height()).c_str());
    surface_ = CreateSurfaceForWidget(widget);
    swapchain_->reset(surface_.get());
    ResetSwapchain();
  }
}
}  // namespace azer
