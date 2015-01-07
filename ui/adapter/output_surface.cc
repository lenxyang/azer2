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
    Canvas2DPtr ptr = device_->GetCanvas();
    TexturePtr tex = ptr->GetTexture();
    RenderSystem* rs = RenderSystem::Current();
    Texture::Options opt = tex->option();
    opt.target = azer::Texture::kShaderResource;
    surface_->texture_.reset(rs->CreateTexture(opt));
	LOG(ERROR) << "id: " << ptr->GetTexID();
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
    , renderer_(rs->GetDefaultRenderer()) {
  Azer2DDevice* azer_device = GetOutputDevice();
  proxy_ = new OutputDeviceProxy(azer_device, this);
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

void DrawCanvas(SkCanvas* canvas) {
  SkString text("Hello, Skia World");
  SkPaint paint;
  paint.setARGB(255, 0, 0, 0);
  paint.setAntiAlias(true);
  paint.setTextSize(SkIntToScalar(30));
  SkScalar x = 80, y = 60;
  canvas->drawText(text.c_str(), text.size(), x, y, paint);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(10);

  SkRect rect;
  paint.setARGB(255, 0, 0, 0);
  rect.set(50, 100, 200, 200);
  canvas->drawRoundRect(rect, 20, 20, paint);

  canvas->drawLine(10, 300, 300, 300, paint);
  canvas->drawCircle(100, 400, 50, paint);
  canvas->flush();
}

void Azer2DOutputSurface::TextureCopy() {
  Azer2DDevice* device = GetOutputDevice();
  Canvas2DPtr ptr = device->GetCanvas();
  TexturePtr tex = ptr->GetTexture();
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
