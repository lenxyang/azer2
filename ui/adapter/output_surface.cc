#include "azer/ui/adapter/output_surface.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/ui/adapter/output_device.h"

#include "SkCanvas.h"

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
    /*
    Canvas2DPtr ptr = device_->GetCanvas();
    TexturePtr tex = ptr->GetTexture();
    RenderSystem* rs = RenderSystem::Current();
    Texture::Options opt = tex->option();
    opt.target = azer::Texture::kShaderResource;
    surface_->texture_.reset(rs->CreateTexture(opt));
    */
  }

 private:
  Azer2DDevice* device_;
  Azer2DOutputSurface* surface_;
  DISALLOW_COPY_AND_ASSIGN(OutputDeviceProxy);
};

Azer2DOutputSurface::Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                                         RenderSystem* rs)
    : cc::OutputSurface(device.Pass())
    , weak_ptr_factory_(this)
    , render_system_(rs)
    , renderer_(rs->GetDefaultRenderer()) {
  Azer2DDevice* azer_device = GetOutputDevice();
  proxy_ = new OutputDeviceProxy(azer_device, this);
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

void Azer2DOutputSurface::TextureCopy() {
}

void Azer2DOutputSurface::PixelsCopy() {
  Azer2DDevice* device = GetOutputDevice();
  Canvas2DPtr ptr = device->GetCanvas();
  TexturePtr tex = ptr->GetTexture();
  Texture::Options opt = tex->option();
  opt.target = azer::Texture::kShaderResource;

  SkCanvas* canvas = ptr->GetSkCanvas();
  int32 size = canvas->imageInfo().width() * canvas->imageInfo().height() * 4;
  ImageDataPtr imagedata(new ImageData(canvas->imageInfo().width(),
                                       canvas->imageInfo().height()));
  canvas->readPixels(canvas->imageInfo(),
                     imagedata->data(),
                     canvas->imageInfo().width() * 4,
                     0, 0);
  Image image(imagedata, Image::k2D);
  texture_.reset(render_system_->CreateTexture(opt, &image));
}

// cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  render_system_->GetContext2D()->finish();
  PixelsCopy();
  client_->DidSwapBuffers();
}

Azer2DDevice* Azer2DOutputSurface::GetOutputDevice() {
  Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
  return device;
}
}  // namespace azer
