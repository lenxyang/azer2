#include "azer/ui/adapter/output_surface.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/ui/adapter/output_device.h"

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
    Canvas2DPtr ptr = device_->GetCanvas();
    TexturePtr tex = ptr->GetTexture();
    RenderSystem* rs = RenderSystem::Current();
    Texture::Options opt = tex->option();
    opt.target = azer::Texture::kShaderResource;
    surface_->texture_.reset(rs->CreateTexture(opt));
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

// Cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  render_system_->GetContext2D()->finish();
  Azer2DDevice* device = GetOutputDevice();
  Canvas2DPtr ptr = device->GetCanvas();
  TexturePtr tex = ptr->CreateSharedTexture();

  static bool save = false;
  if (!save) {
	tex->Save(::base::FilePath(FILE_PATH_LITERAL("ui_adapter.png")));
  }

  tex->CopyTo(texture_.get());
  client_->DidSwapBuffers();
}

Azer2DDevice* Azer2DOutputSurface::GetOutputDevice() {
  Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
  return device;
}
}  // namespace azer
