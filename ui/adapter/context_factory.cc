#include "azer/ui/adapter/context_factory.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/threading/thread.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/context_provider.h"
#include "cc/resources/shared_bitmap_manager.h"
#include "cc/surfaces/surface_id_allocator.h"
#include "cc/raster/single_thread_task_graph_runner.h"
#include "gpu/command_buffer/service/image_factory.h"
#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/reflector.h"
#include "ui/views/widget/widget.h"
#include "azer/azer.h"
#include "azer/ui/adapter/output_device.h"
#include "azer/ui/adapter/output_surface.h"
#include "azer/ui/adapter/util.h"
#include "third_party/skia/include/gpu/gl/GrGLTypes.h"

#include <GLES2/gl2.h>

namespace azer {
namespace {
class SimpleTaskGraphRunner : public cc::SingleThreadTaskGraphRunner {
 public:
  SimpleTaskGraphRunner() {
    Start("SimpleTaskGraphRunner", base::SimpleThread::Options());
  }

  ~SimpleTaskGraphRunner() override {
    Shutdown();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SimpleTaskGraphRunner);
};

class SimpleSharedBitmapManager : public cc::SharedBitmapManager {
 public:
  SimpleSharedBitmapManager();
  ~SimpleSharedBitmapManager() override;

  std::unique_ptr<cc::SharedBitmap> AllocateSharedBitmap(
      const gfx::Size& size) override;

  std::unique_ptr<cc::SharedBitmap> GetSharedBitmapFromId(
      const gfx::Size&,
      const cc::SharedBitmapId& id) override;

 private:
  base::Lock lock_;
  std::map<cc::SharedBitmapId, base::SharedMemory*> bitmap_map_;
};

class AzerImageFactory : public gpu::ImageFactory {
 public:
  AzerImageFactory();
  ~AzerImageFactory() override;

  // Overridden from gpu::ImageFactory:
  scoped_refptr<gl::GLImage> CreateImageForGpuMemoryBuffer(
      const gfx::GpuMemoryBufferHandle& handle,
      const gfx::Size& size,
      gfx::BufferFormat format,
      unsigned internalformat,
      int client_id) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(AzerImageFactory);
};
}

UIContextFactory::UIContextFactory()
    : next_surface_id_namespace_(1u) {
  task_graph_runner_.reset(new SimpleTaskGraphRunner);
  shared_bitmap_manager_.reset(new SimpleSharedBitmapManager);
}

UIContextFactory::~UIContextFactory() {
}

void UIContextFactory::CreateOutputSurface(
    base::WeakPtr<ui::Compositor> compositor) {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  CHECK(NULL != rs);

  gfx::AcceleratedWidget acc_widget = compositor->widget();
  views::Widget* widget = GetWidgetForWindowTreeHost(acc_widget);
  CHECK(widget);

  // For the window not need to render 3D, it will call Present itself
  // after EndPainter, so create for SwapchainOutputDevice for that
  // and the window include 3D render, it need to draw 3d. the UI output
  // is just overlay of the windows.
  std::unique_ptr<cc::SoftwareOutputDevice> device;
  if (!IsRendererWindow(widget)) {
    device.reset(new SwapchainOutputDevice(widget));
  } else {
    device.reset(new TextureOutputDevice(widget));
  }

  std::unique_ptr<Azer2DOutputSurface> surface(new Azer2DOutputSurface(
      std::move(device), compositor->task_runner().get()));
  compositor->SetOutputSurface(std::move(surface));
}

namespace {
class FakeReflector : public ui::Reflector {
 public:
  FakeReflector() {}
  ~FakeReflector() override {}
  void OnMirroringCompositorResized() override {}
  void AddMirroringLayer(ui::Layer* layer) override {}
  void RemoveMirroringLayer(ui::Layer* layer) override {}
};
}  // namespace
std::unique_ptr<ui::Reflector> UIContextFactory::CreateReflector(
    ui::Compositor* mirroed_compositor,
    ui::Layer* mirroring_layer) {
  return base::WrapUnique(new FakeReflector);
}
void UIContextFactory::RemoveReflector(ui::Reflector* reflector) {
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
  return shared_bitmap_manager_.get();
}

gpu::GpuMemoryBufferManager* UIContextFactory::GetGpuMemoryBufferManager() {
  return NULL;
}

std::unique_ptr<cc::SurfaceIdAllocator>
UIContextFactory::CreateSurfaceIdAllocator() {
  return std::unique_ptr<cc::SurfaceIdAllocator>(
      new cc::SurfaceIdAllocator(next_surface_id_namespace_++));
}

uint32_t UIContextFactory::GetImageTextureTarget(gfx::BufferFormat format,
                                                 gfx::BufferUsage usage) {
  return GL_TEXTURE_2D;
}

cc::TaskGraphRunner* UIContextFactory::GetTaskGraphRunner() {
  return task_graph_runner_.get();
}

cc::SurfaceManager* UIContextFactory::GetSurfaceManager() {
  return NULL;
}

void UIContextFactory::ResizeDisplay(ui::Compositor* compositor,
                                     const gfx::Size& size) {
}


namespace {
class OwnedSharedBitmap : public cc::SharedBitmap {
 public:
  OwnedSharedBitmap(std::unique_ptr<base::SharedMemory> shared_memory,
                    const cc::SharedBitmapId& id)
      : SharedBitmap(static_cast<uint8_t*>(shared_memory->memory()), id),
        shared_memory_(std::move(shared_memory)) {}

  ~OwnedSharedBitmap() override {}

 private:
  std::unique_ptr<base::SharedMemory> shared_memory_;
};

}  // namespace

SimpleSharedBitmapManager::SimpleSharedBitmapManager() {}

SimpleSharedBitmapManager::~SimpleSharedBitmapManager() {}

std::unique_ptr<cc::SharedBitmap> SimpleSharedBitmapManager::AllocateSharedBitmap(
    const gfx::Size& size) {
  base::AutoLock lock(lock_);
  std::unique_ptr<base::SharedMemory> memory(new base::SharedMemory);
  memory->CreateAndMapAnonymous(size.GetArea() * 4);
  cc::SharedBitmapId id = cc::SharedBitmap::GenerateId();
  bitmap_map_[id] = memory.get();
  return base::WrapUnique(new OwnedSharedBitmap(std::move(memory), id));
}

std::unique_ptr<cc::SharedBitmap> SimpleSharedBitmapManager::GetSharedBitmapFromId(
    const gfx::Size&,
    const cc::SharedBitmapId& id) {
  base::AutoLock lock(lock_);
  if (bitmap_map_.find(id) == bitmap_map_.end())
    return nullptr;
  uint8_t* pixels = static_cast<uint8_t*>(bitmap_map_[id]->memory());
  return base::WrapUnique(new cc::SharedBitmap(pixels, id));
}
}  // namespace azer
