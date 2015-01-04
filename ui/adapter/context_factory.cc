#include "azer/ui/adapter/context_factory.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/threading/thread.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/context_provider.h"

#include "cc/surfaces/surface_id_allocator.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/reflector.h"
#include "azer/render/render.h"

namespace azer {

UIContextFactory::UIContextFactory()
    : next_surface_id_namespace_(1u) 
    , output_surface_(NULL) {
}

UIContextFactory::~UIContextFactory() {
}

void UIContextFactory::CreateOutputSurface(
    base::WeakPtr<ui::Compositor> compositor,
    bool software_fallback) {
  CHECK(NULL == output_surface_);
  RenderSystem* rs = RenderSystem::Current();
  DCHECK(NULL != rs);
  scoped_ptr<cc::SoftwareOutputDevice> device(new Azer2DDevice(rs->GetContext2D()));
  scoped_ptr<Azer2DOutputSurface> surface(
      new Azer2DOutputSurface(device.Pass(), rs));
  output_surface_ = surface.get();
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

}  // namespace azer
