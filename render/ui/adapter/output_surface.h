#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "cc/output/output_surface_client.h"
#include "cc/output/output_surface.h"

#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/uisbox/adapter/base/output_device.h"

namespace cc {
class SoftwareOutputDevice;
}

class Azer2DOutputSurface : public cc::OutputSurface {
 public:
  Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device);
  ~Azer2DOutputSurface() override;

  azer::TexturePtr& GetTexture() { return texture_;}
 protected:
  // cc::OutputSurface implementation
  void SwapBuffers(cc::CompositorFrame* frame) override;
 private:
  TextureOutputDevice* GetOutputDevice();

  friend class OutputDeviceProxy;
  scoped_refptr<OutputDeviceProxy> proxy_;

  void PixelsCopy();
  void TextureCopy();

  base::WeakPtrFactory<Azer2DOutputSurface> weak_ptr_factory_;
  azer::TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(Azer2DOutputSurface);
};

