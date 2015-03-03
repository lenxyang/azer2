#include "azer/uisbox/adapter/base/output_surface.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/uisbox/adapter/base/output_device.h"

#include "SkCanvas.h"
#include "SkImageInfo.h"
#include "SkString.h"

class OutputDeviceProxy:  public ::base::RefCounted<OutputDeviceProxy> {
 public:
  OutputDeviceProxy(TextureOutputDevice* device, Azer2DOutputSurface* surface)
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
  TextureOutputDevice* device_;
  Azer2DOutputSurface* surface_;
  DISALLOW_COPY_AND_ASSIGN(OutputDeviceProxy);
};

Azer2DOutputSurface::Azer2DOutputSurface(
    scoped_ptr<cc::SoftwareOutputDevice> device)
    : cc::OutputSurface(device.Pass())
    , weak_ptr_factory_(this) {
  TextureOutputDevice* azer_device = GetOutputDevice();
  proxy_ = new OutputDeviceProxy(azer_device, this);
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

void Azer2DOutputSurface::TextureCopy() {
  TextureOutputDevice* device = GetOutputDevice();
  azer::Canvas2DPtr ptr = device->GetCanvas();
  texture_ = ptr->GetTexture();
}


// cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  TextureCopy();
  
  client_->DidSwapBuffers();
  OutputSurface::OnSwapBuffersComplete();
}

TextureOutputDevice* Azer2DOutputSurface::GetOutputDevice() {
  TextureOutputDevice* device = (TextureOutputDevice*)(this->software_device());
  return device;
}
