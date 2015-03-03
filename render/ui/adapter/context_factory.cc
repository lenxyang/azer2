#include "azer/uisbox/adapter/base/context_factory.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/threading/thread.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/context_provider.h"

#include "cc/surfaces/surface_id_allocator.h"
#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/reflector.h"
#include "ui/views/widget/widget.h"
#include "azer/render/render.h"
#include "azer/render/app_context.h"
#include "azer/uisbox/adapter/base/widget_util.h"
#include "azer/uisbox/adapter/base/swapchain_output_device.h"

UIContextFactory::UIContextFactory()
    : next_surface_id_namespace_(1u) {
}

UIContextFactory::~UIContextFactory() {
}

void UIContextFactory::CreateOutputSurface(
    base::WeakPtr<ui::Compositor> compositor,
    bool software_fallback) {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  DCHECK(NULL != rs);

  gfx::AcceleratedWidget acc_widget = compositor->widget();
  views::Widget* widget = GetWidgetForWindowTreeHost(acc_widget);
  CHECK(widget);

  scoped_ptr<cc::SoftwareOutputDevice> device;
  if (IsWidgetRendererWindow(widget)) {
    device.reset(new TextureOutputDevice(rs, widget));
  } else {
    device.reset(new SwapchainOutputDevice(rs, widget));
  }

  scoped_ptr<Azer2DOutputSurface> surface(new Azer2DOutputSurface(device.Pass()));
  compositor->SetOutputSurface(surface.Pass());
}

scoped_refptr<ui::Reflector> UIContextFactory::CreateReflector(
    ui::Compositor* mirroed_compositor,
    ui::Layer* mirroring_layer) {
  return new ui::Reflector();
}
void UIContextFactory::RemoveReflector(
    scoped_refptr<ui::Reflector> reflector) {
}

scoped_refptr<cc::ContextProvider>
UIContextFactory::SharedMainThreadContextProvider() {
  return scoped_refptr<cc::ContextProvider>();
}

void UIContextFactory::RemoveCompositor(ui::Compositor* compositor) {
}

bool UIContextFactory::DoesCreateTestContexts() {
  return false;
}

cc::SharedBitmapManager* UIContextFactory::GetSharedBitmapManager() {
  return NULL;
}

gpu::GpuMemoryBufferManager* UIContextFactory::GetGpuMemoryBufferManager() {
  return NULL;
}
base::MessageLoopProxy* UIContextFactory::GetCompositorMessageLoop() {
  return NULL;
}
scoped_ptr<cc::SurfaceIdAllocator>
UIContextFactory::CreateSurfaceIdAllocator() {
  return make_scoped_ptr(
      new cc::SurfaceIdAllocator(next_surface_id_namespace_++));
}

