#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/output_surface.h"

#include "azer/base/export.h"

namespace cc {
class SoftwareOutputDevice;
}

namespace azer {

class RenderSystem;
class Renderer;

class AZER_EXPORT Azer2DOutputSurface : public cc::OutputSurface {
 public:
  Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                      RenderSystem* rs);
  ~Azer2DOutputSurface() override;

  // cc::OutputSurface implementation
  void SwapBuffers(cc::CompositorFrame* frame) override;
 private:
  base::WeakPtrFactory<Azer2DOutputSurface> weak_ptr_factory_;
  RenderSystem* render_system_;
  Renderer* renderer_;
  std::unique_ptr<azer::Overlay> overlay_;
  DISALLOW_COPY_AND_ASSIGN(Azer2DOutputSurface);
};

}  // namespace azer
