#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "cc/scheduler/begin_frame_source.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/output_surface.h"

#include "azer/base/export.h"
#include "azer/azer.h"
#include "azer/ui/adapter/output_device.h"

namespace cc {
class BeginFrameSource;
class ContextProvider;
class SoftwareOutputDevice;
}

namespace azer {

class TextureOutputDevice;
class Azer2DOutputSurface : public cc::OutputSurface {
 public:
  Azer2DOutputSurface(std::unique_ptr<cc::SoftwareOutputDevice> device,
                      base::SingleThreadTaskRunner* task_runner);
  ~Azer2DOutputSurface() override;

  azer::TexturePtr& GetTexture() { return texture_;}
 protected:
  // cc::OutputSurface implementation
  bool BindToClient(cc::OutputSurfaceClient* client) override;
  void SwapBuffers(cc::CompositorFrame* frame) override;
 private:
  TextureOutputDevice* GetOutputDevice();
  void OnDeviceResize();

  void PixelsCopy();
  void TextureCopy();

  // True when BeginFrame scheduling is enabled.
  bool use_begin_frame_scheduling_;
  std::unique_ptr<cc::SyntheticBeginFrameSource> synthetic_begin_frame_source_;
  azer::TexturePtr texture_;

  ::base::WeakPtrFactory<Azer2DOutputSurface> weak_ptr_factory_;
  int id_;
  DISALLOW_COPY_AND_ASSIGN(Azer2DOutputSurface);
};

}  // namespace azer
