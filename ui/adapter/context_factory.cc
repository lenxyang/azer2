#include "azer/ui/adapter/context_factory.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/threading/thread.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/context_provider.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/output_surface.h"
#include "cc/surfaces/surface_id_allocator.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/reflector.h"

#include "azer/ui/adapter/output_device.h"
#include "azer/render/render.h"

namespace azer {

// An OutputSurface implementation that directly draws and swaps to an actual
// GL surface.
class DirectOutputSurface : public cc::OutputSurface {
 public:
  explicit DirectOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                               RenderSystem* rs)
      : cc::OutputSurface(device.Pass())
      , weak_ptr_factory_(this)
      , render_system_(rs)
      , renderer_(rs->GetDefaultRenderer()) {
    overlay_.reset(rs->CreateOverlay(gfx::RectF(-1.0f, -1.0f, 1.0f, 1.0f)));
    overlay_->EnableBlending(true);
  }
  
  ~DirectOutputSurface() override {
  }

  // cc::OutputSurface implementation
  void SwapBuffers(cc::CompositorFrame* frame) override {
    client_->DidSwapBuffers();

    render_system_->GetContext2D()->finish();
    Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
    Canvas2DPtr canvas = device->GetCanvas();
    overlay_->SetTexture(canvas->GetTexture());
    overlay_->Render(renderer_);
  }

 private:
  base::WeakPtrFactory<DirectOutputSurface> weak_ptr_factory_;
  RenderSystem* render_system_;
  Renderer* renderer_;
  std::unique_ptr<azer::Overlay> overlay_;
  DISALLOW_COPY_AND_ASSIGN(DirectOutputSurface);
};

UIContextFactory::UIContextFactory()
    : next_surface_id_namespace_(1u) {
}

UIContextFactory::~UIContextFactory() {
}

void UIContextFactory::CreateOutputSurface(
    base::WeakPtr<ui::Compositor> compositor,
    bool software_fallback) {
  RenderSystem* rs = RenderSystem::Current();
  DCHECK(NULL != rs);
  scoped_ptr<cc::SoftwareOutputDevice> device(new Azer2DDevice(rs->GetContext2D()));
  scoped_ptr<DirectOutputSurface> surface(
      new DirectOutputSurface(device.Pass(), rs));
  compositor->SetOutputSurface(surface.Pass());

  // surface_context_provider_ = context_provider;
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

}  // namespace azer
