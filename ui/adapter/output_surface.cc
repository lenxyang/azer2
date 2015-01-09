#include "azer/ui/adapter/output_surface.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/ui/adapter/output_device.h"

#include "SkCanvas.h"
#include "SkImageInfo.h"
#include "SkString.h"

namespace azer {

class OutputDeviceProxy:  public ::base::RefCounted<OutputDeviceProxy> {
 public:
  OutputDeviceProxy(Azer2DDevice* device, Azer2DOutputSurface* surface)
      : device_(device)
      , surface_(surface) {
    DCHECK(NULL != device);
    DCHECK(NULL != surface);
    base::Callback<void(void)> callback = 
        ::base::Bind(&OutputDeviceProxy::OnDeviceResize, this);
    device->SetResizeCallback(callback);
  }

  void OnDeviceResize() {
    surface_->texture_ = device_->GetCanvas()->GetTexture();
  }

 private:
  Azer2DDevice* device_;
  Azer2DOutputSurface* surface_;
  DISALLOW_COPY_AND_ASSIGN(OutputDeviceProxy);
};

Azer2DOutputSurface::Azer2DOutputSurface(
    scoped_ptr<cc::SoftwareOutputDevice> device,
    RenderSystem* rs)
    : cc::OutputSurface(device.Pass())
    , weak_ptr_factory_(this)
    , render_system_(rs)
    , renderer_(rs->GetSwapchainRenderer()) {
  Azer2DDevice* azer_device = GetOutputDevice();
  proxy_ = new OutputDeviceProxy(azer_device, this);
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

void Azer2DOutputSurface::TextureCopy() {
  Azer2DDevice* device = GetOutputDevice();
  Canvas2DPtr ptr = device->GetCanvas();
  texture_ = ptr->GetTexture();
}


// cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  TextureCopy();
  client_->DidSwapBuffers();
}

Azer2DDevice* Azer2DOutputSurface::GetOutputDevice() {
  Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
  return device;
}
}  // namespace azer
