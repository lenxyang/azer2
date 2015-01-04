#include "azer/ui/adapter/output_surface.h"

#include "azer/ui/adapter/output_device.h"

namespace azer {
Azer2DOutputSurface::Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                                         RenderSystem* rs)
    : cc::OutputSurface(device.Pass())
    , weak_ptr_factory_(this)
    , render_system_(rs)
    , renderer_(rs->GetDefaultRenderer()) {
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

// cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  client_->DidSwapBuffers();
  render_system_->GetContext2D()->finish();
  static bool save = false;
  if (!save) {
    Azer2DDevice* device = GetOutputDevice();
    Canvas2DPtr ptr = device->GetCanvas();
    ptr->Save(::base::FilePath(FILE_PATH_LITERAL("ui_adapter.png")));
  }
}

Azer2DDevice* Azer2DOutputSurface::GetOutputDevice() {
  Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
  return device;
}
}  // namespace azer
