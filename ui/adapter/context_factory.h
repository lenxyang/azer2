#pragma once

#include "ui/compositor/compositor.h"

namespace base {
class Thread;
}

namespace cc {
class SurfaceManager;
class SharedBitmapManager;
class TaskGraphRunner;
}

namespace azer {

class RenderSystem;
class Renderer;
}  // namespace azer

namespace azer {
class UIContextFactory : public ui::ContextFactory {
 public:
  UIContextFactory();
  ~UIContextFactory() override;
  // ContextFactory implementation
  void CreateOutputSurface(base::WeakPtr<ui::Compositor> compositor) override;
  std::unique_ptr<ui::Reflector> CreateReflector(ui::Compositor* mirrored_compositor,
                                               ui::Layer* mirroring_layer) override;
  void RemoveReflector(ui::Reflector* reflector) override;
  scoped_refptr<cc::ContextProvider> SharedMainThreadContextProvider() override;
  void RemoveCompositor(ui::Compositor* compositor) override;
  bool DoesCreateTestContexts() override;
  uint32_t GetImageTextureTarget(gfx::BufferFormat format,
                                 gfx::BufferUsage usage) override;
  cc::SharedBitmapManager* GetSharedBitmapManager() override;
  gpu::GpuMemoryBufferManager* GetGpuMemoryBufferManager() override;
  cc::TaskGraphRunner* GetTaskGraphRunner() override;
  std::unique_ptr<cc::SurfaceIdAllocator> CreateSurfaceIdAllocator() override;

  cc::SurfaceManager* GetSurfaceManager() override;
  void ResizeDisplay(ui::Compositor* compositor,
                     const gfx::Size& size) override;
  void SetAuthoritativeVSyncInterval(ui::Compositor* compositor,
                                     base::TimeDelta interval) override {}
  void SetOutputIsSecure(ui::Compositor* compositor, bool secure) override {}
 private:
  uint32_t next_surface_id_namespace_;
  std::unique_ptr<cc::SharedBitmapManager> shared_bitmap_manager_;
  std::unique_ptr<cc::SingleThreadTaskGraphRunner> task_graph_runner_;
  DISALLOW_COPY_AND_ASSIGN(UIContextFactory);
};


}  // namespace azer
