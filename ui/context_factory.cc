#include "azer/uisbox/view/base/context_factory.h"

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

#include "azer/uisbox/view/base/output_device.h"


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

    static bool stored = false;
    if (!stored) {
      OutputDevice* device = (OutputDevice*)software_device_.get();
      device->save(::base::FilePath(FILE_PATH_LITERAL("uisbox_view_simple.png")));
      stored = true;
    }
  }

 private:
  base::WeakPtrFactory<DirectOutputSurface> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(DirectOutputSurface);
};

MyContextFactory::MyContextFactory()
    : next_surface_id_namespace_(1u) {
}

MyContextFactory::~MyContextFactory() {
}

void MyContextFactory::CreateOutputSurface(
    base::WeakPtr<ui::Compositor> compositor,
    bool software_fallback) {
  scoped_ptr<cc::SoftwareOutputDevice> device(new OutputDevice);
  compositor->SetOutputSurface(
      make_scoped_ptr(new DirectOutputSurface(device.Pass())));

  // surface_context_provider_ = context_provider;
}
scoped_refptr<ui::Reflector> MyContextFactory::CreateReflector(
    ui::Compositor* mirroed_compositor,
    ui::Layer* mirroring_layer) {
  return new ui::Reflector();
}
void MyContextFactory::RemoveReflector(
    scoped_refptr<ui::Reflector> reflector) {
}

scoped_refptr<cc::ContextProvider>
MyContextFactory::SharedMainThreadContextProvider() {
  return scoped_refptr<cc::ContextProvider>();
}

void MyContextFactory::RemoveCompositor(ui::Compositor* compositor) {
}

bool MyContextFactory::DoesCreateTestContexts() {
  return false;
}

cc::SharedBitmapManager* MyContextFactory::GetSharedBitmapManager() {
  return NULL;
}

gpu::GpuMemoryBufferManager* MyContextFactory::GetGpuMemoryBufferManager() {
  return NULL;
}
base::MessageLoopProxy* MyContextFactory::GetCompositorMessageLoop() {
  return NULL;
}
scoped_ptr<cc::SurfaceIdAllocator>
MyContextFactory::CreateSurfaceIdAllocator() {
  return make_scoped_ptr(
      new cc::SurfaceIdAllocator(next_surface_id_namespace_++));
}
