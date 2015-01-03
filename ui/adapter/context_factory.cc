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

namespace azer {

// An OutputSurface implementation that directly draws and swaps to an actual
// GL surface.
class DirectOutputSurface : public cc::OutputSurface {
 public:
  explicit DirectOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device)
      : cc::OutputSurface(device.Pass()),
        weak_ptr_factory_(this) {}
  ~DirectOutputSurface() override {}
  // cc::OutputSurface implementation
  void SwapBuffers(cc::CompositorFrame* frame) override {
    client_->DidSwapBuffers();
  }

 private:
  base::WeakPtrFactory<DirectOutputSurface> weak_ptr_factory_;
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
  scoped_ptr<cc::SoftwareOutputDevice> device(new Azer2DDevice);
  compositor->SetOutputSurface(
      make_scoped_ptr(new DirectOutputSurface(device.Pass())));

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
