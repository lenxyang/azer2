#pragma once

#include "ui/compositor/compositor.h"
#include "azer/base/export.h"

namespace base {
class Thread;
}

namespace azer {

class AZER_EXPORT UIContextFactory : public ui::ContextFactory {
 public:
  UIContextFactory();
  ~UIContextFactory() override;
  // ContextFactory implementation
  void CreateOutputSurface(base::WeakPtr<ui::Compositor> compositor,
                           bool software_fallback) override;
  scoped_refptr<ui::Reflector> CreateReflector(ui::Compositor* mirrored_compositor,
                                           ui::Layer* mirroring_layer) override;
  void RemoveReflector(scoped_refptr<ui::Reflector> reflector) override;
  scoped_refptr<cc::ContextProvider> SharedMainThreadContextProvider() override;
  void RemoveCompositor(ui::Compositor* compositor) override;
  bool DoesCreateTestContexts() override;
  cc::SharedBitmapManager* GetSharedBitmapManager() override;
  gpu::GpuMemoryBufferManager* GetGpuMemoryBufferManager() override;
  base::MessageLoopProxy* GetCompositorMessageLoop() override;
  scoped_ptr<cc::SurfaceIdAllocator> CreateSurfaceIdAllocator() override;

 private:
  uint32_t next_surface_id_namespace_;
  DISALLOW_COPY_AND_ASSIGN(UIContextFactory);
};

}  // namespace azer
