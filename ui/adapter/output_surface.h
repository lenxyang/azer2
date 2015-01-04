#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/output_surface.h"

#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/ui/adapter/output_device.h"

namespace cc {
class SoftwareOutputDevice;
}

namespace azer {

class AZER_EXPORT Azer2DOutputSurface : public cc::OutputSurface {
 public:
  Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                      RenderSystem* rs);
  ~Azer2DOutputSurface() override;

  Azer2DDevice* GetOutputDevice();
 protected:
  // cc::OutputSurface implementation
  void SwapBuffers(cc::CompositorFrame* frame) override;
 private:
  base::WeakPtrFactory<Azer2DOutputSurface> weak_ptr_factory_;
  RenderSystem* render_system_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(Azer2DOutputSurface);
};

}  // namespace azer
